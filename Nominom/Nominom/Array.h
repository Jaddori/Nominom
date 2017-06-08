#pragma once

#define DEFAULT_SIZE 10

template<typename T>
class Array
{
public:
	Array( int initialCapacity = DEFAULT_SIZE )
		: capacity( initialCapacity ), size( 0 )
	{
		data = new T[capacity];
	}

	Array( const Array& ref )
		: capacity( ref.capacity ), size( ref.size )
	{
		data = new T[capacity];

		for( int i=0; i<ref.size; i++ )
		{
			data[i] = ref.data[i];
		}
	}

	~Array()
	{
		delete[] data;
	}

	Array& operator=( const Array& ref )
	{
		delete[] data;
		
		size = ref.size;
		capacity = ref.capacity;
		data = new T[capacity];

		for( int i=0; i<ref.size; i++ )
		{
			data[i] = ref.data[i];
		}

		return *this;
	}

	T& operator[]( int index )
	{
		return at( index );
	}

	void resize( int newCapacity )
	{
		if( newCapacity > capacity )
		{
			T* tempData = new T[newCapacity];
			for( int i=0; i<size; i++ )
			{
				tempData[i] = data[i];
			}

			delete[] data;
			data = tempData;
			capacity = newCapacity;
		}
	}

	void clear()
	{
		size = 0;
	}

	void add( const T& item )
	{
		if( size >= capacity )
		{
			resize( capacity * 2 );
		}

		data[size] = item;
		size++;
	}

	int find( const T& item )
	{
		int index = -1;
		for( int i=0; i<size && index < 0; i++ )
		{
			if( data[i] == item )
			{
				index = i;
			}
		}

		return index;
	}

	void remove( const T& item )
	{
		int index = find( item );
		if( index >= 0 )
		{
			data[index] = data[size-1];
			size--;
		}
	}

	void removeAt( int index )
	{
		assert( index >= 0 && index < size );

		data[index] = data[size-1];
		size--;
	}

	T& at( int index )
	{
		assert( index >= 0 && index < size );
		return data[index];
	}

	void copy( const Array& ref )
	{
		if( capacity < ref.capacity )
		{
			capacity = ref.capacity;

			delete[] data;
			data = new T[capacity];
		}

		size = ref.size;
		for( int i=0; i<ref.size; i++ )
		{
			data[i] = ref.data[i];
		}
	}

	void fastCopy( const Array& ref )
	{
		if( capacity < ref.capacity )
		{
			capacity = ref.capacity;

			delete[] data;
			data = new T[capacity];
		}

		size = ref.size;
		memcpy( data, ref.data, sizeof(T)*ref.size );
	}

	T* getData()
	{
		return data;
	}

	int getSize() const
	{
		return size;
	}

	int getCapacity() const
	{
		return capacity;
	}

private:
	T* data;
	int size;
	int capacity;
};