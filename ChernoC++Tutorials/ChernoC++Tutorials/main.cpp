#include <iostream>


template<typename T>
class Vector
{
private:
	T* m_Data = nullptr;

	size_t m_Capacity = 0;
	size_t m_Size = 0;

	void ReAlloc(size_t newCapacity)
	{
		T* newBlock = (T*)::operator new(newCapacity * sizeof(T));

		if (newCapacity < m_Size)
			m_Size = newCapacity;

		for (size_t i = 0; i < m_Size; i++)
			newBlock[i] = std::move(m_Data[i]);

		for (size_t i = 0; i < m_Size; i++)
			m_Data[i].~T();

		::operator delete(m_Data, m_Capacity * sizeof(T));
		m_Data = newBlock;
		m_Capacity = newCapacity;
	}
	
public:
	Vector()
	{
		ReAlloc(2);
	}

	~Vector()
	{
		Clear();
		::operator delete(m_Data, m_Capacity * sizeof(T));
	}

	void PushBack(const T& value)
	{
		if (m_Size >= m_Capacity)
		{
			ReAlloc(m_Capacity + m_Capacity / 2);
		} 
		m_Data[m_Size] = value;
		m_Size++;
	}

	size_t Size() const
	{
		return m_Size;
	}

	template<typename... Args>
	T& EmplaceBack(Args&&... args)
	{
		if (m_Size >= m_Capacity)
		{
			ReAlloc(m_Capacity + m_Capacity / 2);
		}

		new(&m_Data[m_Size]) T(std::forward<Args>(args)...);
		return m_Data[m_Size++];
	}

	void PopBack()
	{
		if (m_Size > 0)
		{
			m_Size--;
			m_Data[m_Size].~T();
		}
	}

	void Clear()
	{
		for (size_t i = 0; i < m_Size; i++)
			m_Data[i].~T();

		m_Size = 0;
	}

	const T& operator[](size_t index) const
	{
		if (index >= m_Size) {
			// assert
		}
		return m_Data[index];
	}

	T& operator[](size_t index)
	{
		if (index >= m_Size) {
			// assert
		}
		return m_Data[index];
	}
};

struct Vector3
{
	float x = 0.0f, y = 0.0f, z = 0.0f;
	int* m_MemoryBlock = nullptr;;

	Vector3() { m_MemoryBlock = new int[5]; }
	Vector3(float scalar)
		: x(scalar), y(scalar), z(scalar) {
		m_MemoryBlock = new int[5];
	}
	Vector3(float x, float y, float z)
		: x(x), y(y), z(z) {
		m_MemoryBlock = new int[5];
	}

	Vector3(const Vector3& other) = delete;

	Vector3(Vector3&& other)
		: x(other.x), y(other.y), z(other.z)
	{
		m_MemoryBlock = other.m_MemoryBlock;
		other.m_MemoryBlock = nullptr;
		std::cout << "Move\n";
	}

	~Vector3()
	{
		std::cout << "Destroy\n";
		delete[] m_MemoryBlock;
	}

	Vector3& operator=(const Vector3& other) = delete;

	Vector3& operator=(Vector3&& other)
	{
		m_MemoryBlock = other.m_MemoryBlock;
		other.m_MemoryBlock = nullptr;
		std::cout << "Move\n";
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}
};

void PrintVector(const Vector<Vector3>& vector)
{
	for (size_t i = 0; i < vector.Size(); i++)
		std::cout << vector[i].x << ", " << vector[i].y << ", " << vector[i].z << std::endl;

	std::cout << "-------------------------------\n";
}

int main()
{
	Vector<Vector3> vec
	vec.EmplaceBack(1.0f);
	vec.EmplaceBack(2, 3, 4);
	vec.EmplaceBack(3);
	vec.PopBack();
	vec.PopBack();
	vec.EmplaceBack(5, 2, 0);
	vec.EmplaceBack(1, 7, 9);
	std::cout << "Hello World" << std::endl;
	vec.Clear();

	vec.EmplaceBack(5, 2, 0);
	vec.EmplaceBack(1, 7, 9);

	PrintVector(vec);

	Vector<int> intVector;
	intVector.PushBack(5);
	intVector.EmplaceBack(5);


	std::cin.get();
}
