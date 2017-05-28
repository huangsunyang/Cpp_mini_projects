#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <memory>
#include "stl_memory.hpp"

class student;

class course {
public:
    course(){}
    ~course(){}
    void enroll(std::shared_ptr<student> student_ptr);
    void print_student_list() const;
private:
    std::vector<std::shared_ptr<student>> student_list;
};


class student: public my_enable_shared_from_this<student> {
public:
    student() {}
    student(const std::string &s): name(s){}
    ~student() {}

    void choose_course(course &c) {
        c.enroll(shared_from_this());
    }

    virtual std::string get_name() const {
        return this->name;
    }
private:
    std::string name;
};


void course::enroll(std::shared_ptr<student> student_ptr) {
    assert(student_ptr != nullptr);
    student_list.push_back(student_ptr);
}


void course::print_student_list() const {
    std::cout << "There are " << student_list.size() << " students in this class" << std::endl;
    for (auto student_ptr: student_list) {
        std::cout << student_ptr->get_name() << " ptr_count: " << student_ptr.use_count() << std::endl;
    }
}


int main () {
    std::cout << "hello world" << std::endl;
    course math;
    auto sam = std::make_shared<student>("SAM");
    auto tom = std::make_shared<student>("TOM");

    std::cout << sam.use_count() << std::endl;

    sam->choose_course(math);
    tom->choose_course(math);
    
    math.print_student_list();
    std::cout << "Program ends" << std::endl;
    return 0;
}

