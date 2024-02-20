

#include "Leaderboard.h"
#include <fstream>
#include <iostream>
#include <iomanip> 
#include <ctime>  
using namespace std;
void Leaderboard::insert_new_entry(LeaderboardEntry* new_entry) {
    if (head_leaderboard_entry == nullptr || head_leaderboard_entry->score < new_entry->score) {
        new_entry->next_leaderboard_entry = head_leaderboard_entry;
        head_leaderboard_entry = new_entry;
    } 
    else {
        LeaderboardEntry* current = head_leaderboard_entry;
        int counter = 0;

        while (current->next_leaderboard_entry != nullptr && current->next_leaderboard_entry->score >= new_entry->score && counter < 9) {
            current = current->next_leaderboard_entry;
            counter++;
        }

        new_entry->next_leaderboard_entry = current->next_leaderboard_entry;
        current->next_leaderboard_entry = new_entry;
    }

    // Check if there are more than 10 entries and remove the 11th entry if needed
    LeaderboardEntry* current = head_leaderboard_entry;
    int count = 0;
    while (current->next_leaderboard_entry != nullptr && count < 9) {
        current = current->next_leaderboard_entry;
        count++;
    }

    if (current->next_leaderboard_entry != nullptr) {
        LeaderboardEntry* temp = current->next_leaderboard_entry;
        current->next_leaderboard_entry = nullptr;
        delete temp;
    }
}

void Leaderboard::write_to_file(const string& filename) {
std::ofstream leaderboardfile(filename, std::ios::out);
    if (!leaderboardfile.is_open()) {
        std::cerr << "Error while opening file for writing leaderboard data...\n";
        return;
    }

    LeaderboardEntry* currentEntry = head_leaderboard_entry;

    while (currentEntry != nullptr) {
        leaderboardfile << currentEntry->score << " ";
        leaderboardfile <<  currentEntry->last_played << " ";
        leaderboardfile << currentEntry->player_name << std::endl;

        currentEntry = currentEntry->next_leaderboard_entry;
    }

    leaderboardfile.close();
}

void Leaderboard::read_from_file(const string& filename) {
   std::fstream leaderboardfile(filename, std::ios::in | std::ios::out | std::ios::app);

    if (!leaderboardfile.is_open()) {
        std::cerr << "Error while opening Leaderboard data file...\n";
        return;
    }
    leaderboardfile.clear();
    leaderboardfile.seekg(0, std::ios::beg);
    if (leaderboardfile.peek() == std::ifstream::traits_type::eof()) {
        // File is empty, initialize the leaderboard
        
        head_leaderboard_entry = nullptr;
        leaderboardfile.close();
        return;
    }

    unsigned long score;
    time_t lastPlayed;
    std::string playerName;

    head_leaderboard_entry = nullptr; // Clear the current entries

    // Read the file and construct the linked list
    while (leaderboardfile >> score >> lastPlayed >> playerName) {
        LeaderboardEntry* newEntry = new LeaderboardEntry(score, lastPlayed, playerName);
        newEntry->next_leaderboard_entry = head_leaderboard_entry;
        head_leaderboard_entry = newEntry;
    }

    leaderboardfile.close();

    // Sort the leaderboard entries based on scores in descending order
    sort_leaderboard();

    // Update the 'size' variable if necessary
    update_size();

}


void Leaderboard::sort_leaderboard() {
    bool swapped;
    LeaderboardEntry* temp;
    do {
        swapped = false;
        LeaderboardEntry* current = head_leaderboard_entry;

        while (current->next_leaderboard_entry != nullptr) {
            if (current->score < current->next_leaderboard_entry->score ||
                (current->score == current->next_leaderboard_entry->score &&
                 current->last_played > current->next_leaderboard_entry->last_played)) {
                swap(current, current->next_leaderboard_entry);
                swapped = true;
            }
            current = current->next_leaderboard_entry;
        }
        temp = current;
    } while (swapped);
}



void Leaderboard::swap(LeaderboardEntry* entry1, LeaderboardEntry* entry2) {
    unsigned long tempScore = entry1->score;
    time_t tempLastPlayed = entry1->last_played;
    std::string tempPlayerName = entry1->player_name;

    entry1->score = entry2->score;
    entry1->last_played = entry2->last_played;
    entry1->player_name = entry2->player_name;

    entry2->score = tempScore;
    entry2->last_played = tempLastPlayed;
    entry2->player_name = tempPlayerName;
}

void Leaderboard::update_size() {
    size = 0;
    LeaderboardEntry* current = head_leaderboard_entry;
    while (current != nullptr) {
        size++;
        current = current->next_leaderboard_entry;
    }
}



void Leaderboard::print_leaderboard() {
    LeaderboardEntry* currentEntry = head_leaderboard_entry;
    int number = 1;

        while (currentEntry != nullptr) {
            std::cout << number << ". ";
            std::cout << currentEntry->player_name<< " ";
            std::cout <<currentEntry->score<< " " ;
            std::tm* localTime = std::localtime(&currentEntry->last_played);
            char timeBuffer[20];
            std::strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S/%d.%m.%Y", localTime);
            std::cout << timeBuffer<< std::endl;
            currentEntry = currentEntry->next_leaderboard_entry;
            number++;
        }
    // TODO: Print the current leaderboard status to the standard output in the format specified in the PA instructions
}

Leaderboard::~Leaderboard() {
     LeaderboardEntry* current = head_leaderboard_entry;
    while (current != nullptr) {
        LeaderboardEntry* temp = current;
        current = current->next_leaderboard_entry;
        if (temp!=nullptr){delete temp;};
    }
    head_leaderboard_entry = nullptr; //
    // TODO: Free dynamically allocated memory used for storing leaderboard entries
}
