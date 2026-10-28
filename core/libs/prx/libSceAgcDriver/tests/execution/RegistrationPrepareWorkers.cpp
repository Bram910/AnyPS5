#include "prx/libSceAgcDriver/Execution/include/Driver/Shaders/ShaderRegistry.hpp"
#include "ControlFlow/RequestSerializer.hpp"
#include "CompiledVariant.hpp"
#include "Recompiler.hpp"
#include "Optimization/ResourceProgram.hpp"
#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>

namespace {

void Require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

void Replay(const char* path) {
    std::ifstream file(path, std::ios::binary);
    Require(file.good(), "cannot open captured request");
    const std::string text{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
    auto request = ShaderRecompiler::RequestSerializer{}.Deserialize(text);
    const auto handle = ShaderRecompiler::PrepareShader(request.request);
    const auto& artifact = ShaderRecompiler::GetPreparedArtifact(*handle);
    const auto output = std::filesystem::path(path).filename().string() + ".spv";
    std::ofstream stream(output, std::ios::binary);
    stream.write(reinterpret_cast<const char*>(artifact.spirv.data()), static_cast<std::streamsize>(artifact.spirv.size() * sizeof(std::uint32_t)));
    Require(stream.good(), "cannot write prepared module");
}

}

int main(int argc, char** argv) {
    using namespace AgcDriver::DriverDetail;
    try {
        if (argc == 2 && std::string(argv[1]) == "--invalid") {
            bool rejected = false;
            try {
                SubmitRegistrationPreparation([] {});
            } catch (const std::runtime_error& error) {
                rejected = std::string(error.what()) == "APS5_PREPARE_THREADS must be a positive integer";
            }
            Require(rejected && RegistrationPreparationThreads() == 0, "invalid worker limit created a pool");
            return 0;
        }
        if (argc > 2 && std::string(argv[1]) == "--replay") {
            std::atomic<unsigned> failures{0};
            for (int index = 2; index < argc; ++index) {
                SubmitRegistrationPreparation([path = argv[index], &failures] {
                    try {
                        Replay(path);
                    } catch (const std::exception& error) {
                        std::cerr << error.what() << '\n';
                        ++failures;
                    }
                });
            }
            AwaitRegistrationPreparation();
            std::cout << "prepared " << argc - 2 << " requests with " << RegistrationPreparationThreads() << " workers\n";
            CloseRegistrationPreparation();
            return failures == 0 ? 0 : 1;
        }
        const auto available = std::max(2u, std::thread::hardware_concurrency() / 2u);
        const auto* text = std::getenv("APS5_PREPARE_THREADS");
        const auto expected = text == nullptr ? available : std::min(available, static_cast<unsigned>(std::strtoul(text, nullptr, 10)));
        std::mutex mutex;
        std::condition_variable ready;
        unsigned entered = 0;
        bool release = false;
        std::atomic<unsigned> completed{0};
        for (unsigned index = 0; index < expected + 3; ++index) {
            SubmitRegistrationPreparation([&] {
                std::unique_lock lock(mutex);
                ++entered;
                ready.notify_all();
                ready.wait(lock, [&] { return release; });
                ++completed;
            });
        }
        bool filled;
        unsigned active;
        {
            std::unique_lock lock(mutex);
            filled = ready.wait_for(lock, std::chrono::seconds(5), [&] { return entered >= expected; });
            active = entered;
            release = true;
        }
        ready.notify_all();
        const auto workers = RegistrationPreparationThreads();
        AwaitRegistrationPreparation();
        CloseRegistrationPreparation();
        Require(filled && active == expected && workers == expected, "worker limit did not bound concurrent preparation");
        Require(completed == expected + 3 && RegistrationPreparationThreads() == 0, "queued work was lost or workers survived shutdown");
        std::cout << "preparation worker limit passed: " << expected << '\n';
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
