#include <iostream>
#include <vector>
#include <fstream>
#include <utility>

template <typename T>
struct Matrix {
public:
    Matrix(size_t columnsCount, size_t rowsCount) : columnsCount(columnsCount), rowsCount(rowsCount) {
        this->data = std::vector<std::vector<T>>(columnsCount);
        std::fill(this->data.begin(), this->data.end(), std::vector<T>(rowsCount));
    }

    size_t getColumnsCount(void) const {
        return this->columnsCount;
    }

    size_t getRowsCount(void) const {
        return this->rowsCount;
    }

    decltype(auto) operator[](this auto&& self, size_t index) {
        if (index < 0 || index >= this->columnsCount) {
            std::cerr << "index out of range (Matrix)" << std::endl;
        }
        return std::forward_like<decltype(self)>(this->data[index]);
    }

private:
    std::vector<std::vector<T>> data;
    size_t columnsCount, rowsCount;
};

class BinarySerializer {
public:
    BinarySerializer(const std::string& filePath)
        : file(filePath, std::ios_base::in | std::ios_base::binary | std::ios_base::out) {}

    template <typename T, typename... Args>
    void serialize(T first, Args... args) {
        try {
            file.write(reinterpret_cast<char*>(&first), sizeof(first));
            serialize(args...);
        }
        catch (const std::exception& exception) {
            std::cerr << exception.what() << std::endl;
        }
        std::cout << "data has been deserialized" << std::endl;
    }

    template <typename T, typename... Args>
    void deserialize(T& first, Args&... args) {
        try {
            deserialize(first);
            deserialize(args...);
        }
        catch (const std::exception& exception) {
            std::cerr << exception.what() << std::endl;
        }
        std::cout << "data has been deserialized" << std::endl;
    }

private:
    template <typename T>
    void serialize(T first) {
        file.write(reinterpret_cast<char*>(&first), sizeof(first));
    }

    template <typename T>
    void deserialize(T& first) {
        char buffer[BUFFER_MAX_SIZE] = { 0 };
        file.read(buffer, sizeof(T));

        first = *(static_cast<T*>(buffer));
    }

    constexpr static size_t BUFFER_MAX_SIZE = 50;
    std::fstream file;
};

template <typename T>
class VectorBase {
public:
    VectorBase(void) {
        static_assert(std::is_floating_point<T>::value);
    }

    T* convertToArray(void) const {
        return reinterpret_cast<T*>(this);
    }

    virtual size_t getLength(void) const = 0;

    friend std::ostream& operator<<(std::ostream& ostream, const VectorBase& vector) {
        for (size_t i = 0; i < this->getLength(); i++) {
            ostream << this->convertToArray()[i] << " | ";
        }
        return ostream;
    }
};

template <typename T>
class Vector2D : public VectorBase<T> {
public:
    Vector2D(T x, T y) : x(x), y(y), VectorBase<T>() {}

    size_t getLength(void) const { return 2; }

private:
    T x, y;
};

template <typename T>
class Vector3D : public Vector2D<T> {
public:
    Vector3D(T x, T y, T z) : z(z), Vector2D() {}

    size_t getLength(void) const { return 3; }

private:
    T x, y, z;
};

namespace GeometryFunctions {
    
}; // append some functions

namespace Tools {
    template <typename T>
    void print(const char* separator, const char* end, T first) {
        std::cout << first << end;
    }
    
    template <typename T, typename... Args>
    void print(const char* separator, const char* end, T first, Args... args) {
        std::cout << first << separator;
        print(separator, end, args...);
    }
};

int main(int argc, char* argv[]) {
    BinarySerializer serializer("./serialized.bin");
    Tools::print(" | ", "\n", 4, 5, 3, 4, "Hello world", "bye bye", 434.5);

    Vector2D<double> vector(45.4, 56.45);
    Vector2D<double> vector_2(3.4, 23.434);
    Vector2D<double> vector_3(56.4, 3.233);

    Tools::print("\n", "\n", vector, vector_2, vector_3);

    return 0;
}