#include <iostream>
#include <vector>
#include <fstream>
#include <utility>

template <typename T>
class Matrix {
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

    friend std::ostream& operator<<(std::ostream& ostream, VectorBase& vector) {
        T* array = vector.convertToArray();
        for (size_t i = 0; i < vector.getLength(); i++) {
            ostream << array[i] << " | ";
        }
        return ostream;
    }

    T* convertToArray(void) {
        return reinterpret_cast<T*>(this + 1);
    }

    virtual size_t getLength(void) const = 0;
};

template <typename T>
class Vector2D : public VectorBase<T> {
public:
    Vector2D(T x, T y) : x(x), y(y), VectorBase<T>() {}

    size_t getLength(void) const override { return 2; }

private:
    T x, y;
};

template <typename T>
class Vector3D : public VectorBase<T> {
public:
    Vector3D(T x, T y, T z) : x(x), y(y), z(z), VectorBase<T>() {}

    size_t getLength(void) const override { return 3; }

private:
    T x, y, z;
};

template <typename T>
using ProjectionMatrix = Matrix<T>;

template <typename T>
using RotationXMatrix = Matrix<T>;

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
    Matrix<double> projectionMatrix(4, 4);
    Vector2D vector_1(43.43, 34.43), vector_2(21.2, 34.2), vector_3(1.2, 34.23);
    Vector3D vector3D(32.34, 1.34, 4.43);

    Tools::print("\n", "\n", vector_1, vector_2, vector_3, vector3D);

    return 0;
}
