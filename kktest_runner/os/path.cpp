#include <iostream>

#include <utils/string.hpp>
#include "path.hpp"

using namespace std;

namespace kktest_runner {

const char Path::SEP = '/';
const char* Path::SELF_FOLDER = ".";
const char* Path::PARENT_FOLDER = "..";

Path::Path(const string& path) {
    relative = !startsWith(path, SEP);
    size_t prev = 0, pos = 0;
    while ((pos = path.find(SEP, pos)) != string::npos) {
        string part = path.substr(prev, pos - prev);
        if (!part.empty()) {
            addPart(move(part));
        }
        prev = ++pos;
    }
    if (pos != prev) {
        addPart(path.substr(prev, pos - prev));
    }
}

Path::Path(const Path& other): parts(other.parts), relative(other.relative) {}

Path::Path(Path&& other): parts(move(other.parts)), relative(other.relative) {}

Path& Path::operator=(const Path& other) {
    parts = other.parts;
    relative = other.relative;
    return *this;
}

Path& Path::operator=(Path&& other) {
    parts = move(other.parts);
    relative = other.relative;
    return *this;
}

bool Path::isRelative() const {
    return relative;
}

bool Path::isAbsolute() const {
    return !relative;
}

string Path::last() const {
    if (parts.empty()) {
        throw EmptyPathError();
    }
    return parts.back();
}

string Path::toString() const {
    string ret;
    if (isAbsolute()) {
        ret += SEP;
    }
    for (const string& part: parts) {
        ret += part;
        ret += SEP;
    }
    return ret;
}

vector<string> Path::getParts() const {
    return parts;
}

bool Path::applySpecialPart(const string& part) {
    if (part.empty() || part == SELF_FOLDER) {
        return true;
    }
    if (part == PARENT_FOLDER) {
        if (!parts.empty() && parts.back() != PARENT_FOLDER) {
            parts.pop_back();
        } else {
            parts.push_back(PARENT_FOLDER);
        }
        return true;
    }
    return false;
}

void Path::addPart(const string& part) {
    if (!applySpecialPart(part)) {
        parts.emplace_back(part);
    }
}

void Path::addPart(string&& part) {
    if (!applySpecialPart(part)) {
        parts.emplace_back(part);
    }
}

Path Path::operator+(const Path& other) const {
    return join(*this, other);
}

Path& Path::operator+=(const Path& other) {
    return *this = join(*this, other);
}

Path Path::operator+(const string& other) const {
    return join(*this, other);
}

Path& Path::operator+=(const string& other) {
    return *this = join(*this, other);
}

Path Path::parent() const {
    Path other(*this);
    if (!other.parts.empty()) {
        other.parts.pop_back();
    }
    return other;
}

Path Path::parent(const Path& path) {
    return path.parent();
}

Path& Path::operator--() {
    *this = parent();
    return *this;
}

Path Path::operator--(int) {
    Path other(*this);
    -- *this;
    return other;
}

Path Path::join(const Path& a, const Path& b) {
    Path joined = a;
    for (const string& part: b.parts) {
        joined.addPart(part);
    }
    return joined;
}

Path Path::join(const Path& a, const string& b) {
    return join(a, Path(b));
}

Path Path::join(const string& a, const Path& b) {
    return join(Path(a), b);
}

Path Path::join(const string& a, const string& b) {
    return Path::join(Path(a), Path(b));
}

string Path::key() const {
    return toString();
}

}
