//
// Created by bysou on 2021/12/26.
//

#ifndef BYS_LIB_ARCHIVER_OBJ_H
#define BYS_LIB_ARCHIVER_OBJ_H

#include "archiver.h"
#include <iostream>
#include <vector>

//  1. basic json format
struct Student {
    Student() : name(), age(), height(), canSwim() {}
    Student(const std::string name, unsigned age, double height, bool canSwim) :
            name(name), age(age), height(height), canSwim(canSwim)
    {}

    std::string name;
    unsigned age;
    double height;
    bool canSwim;
};

template <typename Archiver>
Archiver& operator&(Archiver& ar, Student& s) {
    ar.StartObject();
    ar.Member("name") & s.name;
    ar.Member("age") & s.age;
    ar.Member("height") & s.height;
    ar.Member("canSwim") & s.canSwim;
    return ar.EndObject();
}

std::ostream& operator<<(std::ostream& os, const Student& s) {
    return os << s.name << " " << s.age << " " << s.height << " " << s.canSwim;
}
////////////////////////////////////////////////////////////////////////////////
//  2. vector <=> json array
struct Group {
    Group() : groupName(), students() {}
    std::string groupName;
    std::vector<Student> students;
};

template <typename Archiver>
Archiver& operator&(Archiver& ar, Group& g) {
    ar.StartObject();

    ar.Member("groupName");
    ar & g.groupName;

    ar.Member("students");
    size_t studentCount = g.students.size();
    ar.StartArray(&studentCount);
    if (ar.IsReader)
        g.students.resize(studentCount);
    for (size_t i = 0; i < studentCount; i++)
        ar & g.students[i];
    ar.EndArray();

    return ar.EndObject();
}

std::ostream& operator<<(std::ostream& os, const Group& g) {
    os << g.groupName << std::endl;
    for (std::vector<Student>::const_iterator itr = g.students.begin(); itr != g.students.end(); ++itr)
        os << *itr << std::endl;
    return os;
}
////////////////////////////////////////////////////////////////////////////////
//  3. polymorphism & friend

class Shape {
public:
    virtual ~Shape() {}
    virtual const char* GetType() const = 0;
    virtual void Print(std::ostream& os) const = 0;

protected:
    Shape() : x_(), y_() {}
    Shape(double x, double y) : x_(x), y_(y) {}

    template <typename Archiver>
    friend Archiver& operator&(Archiver& ar, Shape& s);

    double x_, y_;
};

template <typename Archiver>
Archiver& operator&(Archiver& ar, Shape& s) {
    ar.Member("x") & s.x_;
    ar.Member("y") & s.y_;
    return ar;
}

class Circle : public Shape {
public:
    Circle() : radius_() {}

    Circle(double x, double y, double radius) : Shape(x, y), radius_(radius) {}

    ~Circle() {}

    const char *GetType() const { return "Circle"; }

    void Print(std::ostream &os) const {
        os << "Circle (" << x_ << ", " << y_ << ")" << " radius = " << radius_;
    }

private:
    template<typename Archiver>
    friend Archiver &operator&(Archiver &ar, Circle &c);

    double radius_;
};


template <typename Archiver>
Archiver& operator&(Archiver& ar, Circle& c) {
    ar & static_cast<Shape&>(c);
    ar.Member("radius") & c.radius_;
    return ar;
}

class Box : public Shape {
public:
    Box() : width_(), height_() {}
    Box(double x, double y, double width, double height) : Shape(x, y), width_(width), height_(height) {}
    ~Box() {}

    const char* GetType() const { return "Box"; }

    void Print(std::ostream& os) const {
        os << "Box (" << x_ << ", " << y_ << ")" << " width = " << width_ << " height = " << height_;
    }

private:
    template <typename Archiver>
    friend Archiver& operator&(Archiver& ar, Box& b);

    double width_, height_;
};

template <typename Archiver>
Archiver& operator&(Archiver& ar, Box& b) {
    ar & static_cast<Shape&>(b);
    ar.Member("width") & b.width_;
    ar.Member("height") & b.height_;
    return ar;
}

class Canvas {
public:
    Canvas() : shapes_() {}
    ~Canvas() { Clear(); }

    void Clear() {
        for (std::vector<Shape*>::iterator itr = shapes_.begin(); itr != shapes_.end(); ++itr)
            delete *itr;
    }

    void AddShape(Shape* shape) { shapes_.push_back(shape); }

    void Print(std::ostream& os) {
        for (std::vector<Shape*>::iterator itr = shapes_.begin(); itr != shapes_.end(); ++itr) {
            (*itr)->Print(os);
            std::cout << std::endl;
        }
    }

private:
    template <typename Archiver>
    friend Archiver& operator&(Archiver& ar, Canvas& c);

    std::vector<Shape*> shapes_;
};

template <typename Archiver>
Archiver& operator&(Archiver& ar, Shape*& shape) {
    std::string type = ar.IsReader ? "" : shape->GetType();
    ar.StartObject();
    ar.Member("type") & type;
    if (type == "Circle") {
        if (ar.IsReader) shape = new Circle;
        ar & static_cast<Circle&>(*shape);
    }
    else if (type == "Box") {
        if (ar.IsReader) shape = new Box;
        ar & static_cast<Box&>(*shape);
    }
    return ar.EndObject();
}

template <typename Archiver>
Archiver& operator&(Archiver& ar, Canvas& c) {
    size_t shapeCount = c.shapes_.size();
    ar.StartArray(&shapeCount);
    if (ar.IsReader) {
        c.Clear();
        c.shapes_.resize(shapeCount);
    }
    for (size_t i = 0; i < shapeCount; i++)
        ar & c.shapes_[i];
    return ar.EndArray();
}

#endif //BYS_LIB_ARCHIVER_OBJ_H
