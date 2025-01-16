//doesnt work
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>

std::vector<std::unordered_map<std::string, std::string>> read_database(const std::string& filename, std::vector<std::string>& strs) {
    std::vector<std::unordered_map<std::string, std::string>> database;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return database;
    }

    std::getline(file, line);
    std::istringstream header_stream(line);
    std::string header;
    while (std::getline(header_stream, header, ',')) {
        strs.push_back(header);
    }
    strs.erase(strs.begin());

    while (std::getline(file, line)) {
        std::unordered_map<std::string, std::string> entry;
        std::istringstream line_stream(line);
        std::string value;
        size_t i = 0;

        while (std::getline(line_stream, value, ',')) {
            entry[strs[i]] = value;
            ++i;
        }
        database.push_back(entry);
    }

    return database;
}

std::string read_sequence(const std::string& filename) {
    std::ifstream file(filename);
    std::string sequence;

    if (file.is_open()) {
        std::getline(file, sequence);
        return sequence;
    }

    return "";
}

int count_str_repeats(const std::string& sequence, const std::string& str_sequence) {
    int max_count = 0;
    size_t str_length = str_sequence.length();

    for (size_t i = 0; i <= sequence.length() - str_length; ++i) {
        int count = 0;
        while (sequence.substr(i, str_length) == str_sequence) {
            count++;
            i += str_length;
        }
        max_count = std::max(max_count, count);
    }

    return max_count;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Usage: program database.txt sequence.txt" << std::endl;
        return 1;
    }

    std::string database_file = argv[1];
    std::string sequence_file = argv[2];

    std::vector<std::string> strs;
    std::vector<std::unordered_map<std::string, std::string>> database = read_database(database_file, strs);
    std::string sequence = read_sequence(sequence_file);

    if (sequence.empty()) {
        std::cerr << "Error reading sequence file." << std::endl;
        return 1;
    }

    std::unordered_map<std::string, int> str_counts;
    for (const auto& str_sequence : strs) {
        str_counts[str_sequence] = count_str_repeats(sequence, str_sequence);
    }

    for (const auto& person : database) {
        bool match = true;
        for (const auto& str_sequence : strs) {
            try {
                int count = std::stoi(person.at(str_sequence));
                if (count != str_counts[str_sequence]) {
                    match = false;
                    break;
                }
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid argument in database entry: " << person.at(str_sequence) << std::endl;
                match = false;
                break;
            } catch (const std::out_of_range& e) {
                std::cerr << "Out of range value in database entry: " << person.at(str_sequence) << std::endl;
                match = false;
                break;
            }
        }
        if (match) {
            std::cout << person.at("name") << std::endl;
            return 0;
        }
    }

    std::cout << "No match" << std::endl;
    return 0;
}
