#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <array>
#include <SDL.h>

template <typename T> requires (std::is_floating_point<T>::value)
class Matrix {
public:
    Matrix(const std::vector<std::vector<T>>&& data)
      : columnsCount(data.size()), rowsCount(data[0].size()), data(std::move(data)) {}

    size_t getColumnsCount(void) const {
        return this->columnsCount;
    }

    size_t getRowsCount(void) const {
        return this->rowsCount;
    }

    template <typename Self>
    decltype(auto) operator[](this Self&& self, size_t index) {
        if (index < 0 || index >= self.columnsCount) {
            std::cerr << "index out of range (Matrix)" << std::endl;
        }
        return std::forward_like<decltype(self)>(self.data[index]);
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
        std::cout << "data has been serialized" << std::endl;
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

        first = *(reinterpret_cast<T*>(buffer));
    }

    constexpr static size_t BUFFER_MAX_SIZE = 50;
    std::fstream file;
};

template <typename T> requires(std::is_floating_point<T>::value)
class VectorBase {
public:
    VectorBase(void) = default;

    friend std::ostream& operator<<(std::ostream& ostream, VectorBase& vector) {
        T* array = vector.convertToArray();
        for (size_t i = 0; i < vector.getLength(); i++) {
            ostream << array[i] << " | ";
        }
        return ostream;
    }

    template <typename Self>
    decltype(auto) convertToArray(this Self&& self) {
        return reinterpret_cast<T*>(&self + 1);
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
    Vector2D vector_2(0.0, 0.0);

    return 0;
}
