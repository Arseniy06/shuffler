#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <random>

class Question{
    public:
    std::string question;
    std::vector<std::string> answers;
    int correct_answer;
    int answers_number;
    int id;
    Question (std::string question, std::vector<std::string> answers, int id){
        this->question = question;
        this->answers = answers;
        correct_answer = 0;
        answers_number = size(answers);
        this->id = id;
    }
    Question(){
        question = "Unidentified question";
        answers = {"Unidentified answer"};
        correct_answer = -1;
    }
    void shuffle (){
        srand(time(NULL));
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist6(0,20);
        std::vector<std::string> new_answers = {};
        std::vector<std::string> tmp_clone = answers;
        std::string original = answers[correct_answer];
        int to_be_moved;
        int new_correct_answer;
        for (int i = 0; i<answers_number;i++){
            
            to_be_moved = dist6(rng)%size(answers);
            new_answers.push_back(answers[to_be_moved]);
            if (answers[to_be_moved] == tmp_clone[correct_answer]){
                new_correct_answer = i;
            }
            answers.erase(answers.begin()+to_be_moved);
        }
        answers = new_answers;
        correct_answer = new_correct_answer;
    }
    void beautify(){
        question = std::to_string(id)+". "+question;
        for (int i = 0; i<answers_number; i++){
            answers[i] = std::to_string(i+1)+ ") " + answers[i];
        }
    }
    std::string que (){
        std::string output = question + "\n\n";
        for (int i = 0; i< answers_number; i++){
            output += answers[i] + "\n";
        }
        output += "\n\n";
        return output;
    }
    std::string ans(){
        std::string output = question + ": " +answers[correct_answer];
        return output;
    }
};

// Trim from the start (in place)
inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// Trim from the end (in place)
inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}
inline void trim(std::string &s) {
    rtrim(s);
    ltrim(s);
}

void demo(){
    Question quest = Question("what",{"1","2","3","4"},1);
    quest.shuffle();
    /*for (int i = 0;i< quest.answers_number;i++){
        std::cout<<quest.answers[i]<<std::endl;
    }
    std::cout<<std::endl<<quest.correct_answer<<std::endl;*/
    std::cout<<quest.que()<<std::endl;
}

void generate_set(std::string set_file, std::vector<Question> questions, int set_size){
    std::ofstream questions_file(set_file+"_questions.txt");
    std::ofstream answers_file(set_file+"_answers.txt");
    if (!questions_file.is_open()||!answers_file.is_open()){
        std::cout<<"Failed to create file "<<set_file<<std::endl;
    }
    std::vector<Question> set;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0,1000);
    for (int i = 0;i<set_size;i++){
        set.push_back(questions[dist6(rng)%size(questions)]);
    }
    for(int i = 0; i<set_size;i++){
        questions_file<<set[i].que()<<std::endl;
        answers_file<<set[i].ans()<<std::endl;
    }
}

int main(){
    int sets_number = 3;
    int sets_size = 30;
    int questions_created = 1;
    std::vector<Question> questions;
    std::ifstream input_file("dataset.txt");
    if (!input_file.is_open()){
        std::cout<<"Error opening the file"<<std::endl;
        return 0;
    }
    std::string line;
    std::string crutch = ".)";
    std::string question;
    std::vector<std::string> answers;
    bool prev_was_answer = 0;
    while (std::getline(input_file, line)) {
        if ((line != "\n")&&(line !=" ")&&(line !="")){
            trim(line);
            if ((line[1] == crutch[0]) || (line[2] == crutch[0])){
                if (prev_was_answer == 1){
                    //Question quest = Question(question, answers);
                    questions.push_back(Question(question,answers,questions_created));
                    answers = {};
                    questions_created++;
                }
                int first = line.find(".");
                question = line.substr(first+1);
                prev_was_answer = 0;
            } else if ((line[1] == crutch[1])){
                int first = line.find(")");
                answers.push_back(line.substr(first+1));
                //std::cout<<line.substr(first+1)<<std::endl;
                //std::cout<<line[1]<<line<<std::endl;
                prev_was_answer = 1;
            }
        }
    }
    input_file.close();
    questions.push_back(Question(question,answers,questions_created));
    for(int i = 0;i<questions_created;i++){
        questions[i].shuffle();
        questions[i].beautify();
    }
    std::ofstream questionsfile ("questions.txt");
    std::ofstream answersfile ("answers.txt");
    if (questionsfile.is_open()&&answersfile.is_open()){
        for (int i = 0;i<questions_created;i++){
            questionsfile<<questions[i].que()<<std::endl;
            answersfile<<questions[i].ans()<<std::endl;
        }
    }
    questionsfile.close();
    answersfile.close();
    for (int i = 1; i<=sets_number;i++){
        generate_set("set"+std::to_string(i),questions,sets_size);
    }
    return 0;
}