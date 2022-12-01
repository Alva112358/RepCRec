/**
* @brief       RepCRec database system project.
* @copyright   New York University.
* @authors     Junhua Liang, Guanqun Yang.
* @date        2022-12-02.
*/

#include <filesystem>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

#include "repcrec/transaction_manager/transaction_manager.h"
using namespace repcrec::transaction_manager;
using namespace repcrec::instruction;
using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
using std::cout;
using std::endl;
using std::string;

static std::string inputFilePath = "./test/input/";
static std::string resultFilePath = "./test/output/";
static std::string outputFilePath = "./test/";

void execute_test_case_check() {
    int all = 0;
    int right = 0;
    for (const auto &dirEntry: recursive_directory_iterator(resultFilePath)) {
        if (dirEntry.is_directory()) {
            continue;
        }
        all++;
        const string fileName = dirEntry.path().filename();
        cout << "Test case: " << fileName << endl;

        std::ifstream resultFile(resultFilePath + fileName);
        std::ifstream outputFile(outputFilePath + fileName);
        string resultLine;
        string outputLine;
        while (std::getline(resultFile, resultLine) && std::getline(outputFile, outputLine)) {
            if (resultLine != outputLine) {
                cout << "\033[1;31mWrong Line\033[0m" << endl;
                cout << "Result: " << resultLine << endl;
                cout << "Your output: " << outputLine << endl;
                break;
            }
        }
        if (resultLine == outputLine) {
            cout << "\033[1;32mPass\033[0m" << endl;
            right++;
        }
        resultFile.close();
        outputFile.close();
    }

    cout << right << " / " << all << " pass" << endl;
}

void execute_advanced_database(const std::string_view &file_name, const std::string &mode) {
    std::ifstream insr_file;
    insr_file.open(inputFilePath + file_name.data(), std::ios::binary);
    std::shared_ptr<std::ofstream> outFile;
    if (mode == "test") {
        outFile = std::make_shared<std::ofstream>(outputFilePath + file_name.data());
        repcrec::site_manager::SiteManager::set_file(outFile);
    }

    std::string insr;
    while (getline(insr_file, insr)) {
        // Skip dump and execute it after finishing all instructions.
        if (insr.starts_with("dump")) {
            continue;
        }
        ++TransactionManager::curr_timestamp;
        TransactionManager::get_instance().add_instruction(std::make_shared<Instruction>(insr), TransactionManager::curr_timestamp);
        TransactionManager::get_instance().execute_instructions(TransactionManager::curr_timestamp);
    }
    insr_file.close();
    while (!TransactionManager::get_instance().is_all_instructions_finished()) {
        TransactionManager::get_instance().execute_instructions(TransactionManager::curr_timestamp);
        ++TransactionManager::curr_timestamp;
    }
    // extra dump
    TransactionManager::get_instance().add_instruction(std::make_shared<Instruction>("dump()"), TransactionManager::curr_timestamp++);
    TransactionManager::get_instance().execute_instructions(TransactionManager::curr_timestamp);

    if (mode == "test") {
        outFile->close();
    }
}


int main(int argc, char **argv) {
    if (argc < 3) {
        throw std::invalid_argument("Please specify your testing file.\n");
    }
    if (string(argv[2]) == "all") {
        for (const auto &dirEntry: recursive_directory_iterator(inputFilePath)) {
            cout<<"---------------------------------------------------"<<endl;
            cout<<"start "<<dirEntry.path().filename()<<endl;
            auto pid = fork();
            if (pid == 0) {
                execute_advanced_database(string(dirEntry.path().filename()), "test");
                return 0;
            } else {
                while (true) {
                    int status;
                    pid_t done = wait(&status);
                    if (done == -1) {
                        if (errno == ECHILD) break;
                    } else {
                        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
                            std::cerr << dirEntry.path().filename()  << " failed" << endl;
                            break;
                        }
                    }
                }
            }
        }
        if (string(argv[1]) == "test") {
            execute_test_case_check();
        }
        return 0;
    } else {
        execute_advanced_database(argv[2], argv[1]);
        if (string(argv[1]) == "test") {
            execute_test_case_check();
        }
    }
    return 0;
}
