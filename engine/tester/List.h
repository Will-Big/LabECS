#pragma once

#include <algorithm>
#include <cassert>

template <typename T>
struct Node
{
	T _data;
	Node<T>* _prev = nullptr;
	Node<T>* _next = nullptr;

	Node(T data) : _data(data) {}

	template <typename... Args>
	Node(Args&&... args) : _data(std::forward<Args>(args)...) {}
};

template <typename T>
class List
{
public:
	class Iterator
	{
	public:
		Iterator(Node<T>* node, Node<T>* head) : _node(node), _head(head) {}

		T& operator*()
		{
			return _node->_data;
		}

		T* operator->()
		{
			return &_node->_data;
		}

		Iterator& operator++()
		{
			assert(_node != _head && "cannot increment end Iterator");

			_node = _node->_next;
			return *this;
		}

		Iterator operator++(int)
		{
			assert(_node != _head && "cannot increment end Iterator");

			Iterator temp = *this;
			_node = _node->_next;

			return temp;
		}

		Iterator& operator--()
		{
			assert(_node->_prev != _head && "cannot decrement begin Iterator");

			_node = _node->_prev;
			return *this;
		}

		Iterator operator--(int)
		{
			assert(_node->_prev != _head && "cannot decrement begin Iterator");

			Iterator temp = *this;
			_node = _node->_prev;

			return temp;
		}

		bool operator==(const Iterator& rhs) const
		{
			return this->_node == rhs._node;
		}

		bool operator!=(const Iterator& rhs) const
		{
			return this->_node != rhs._node;
		}

		bool operator<(const Iterator& rhs) const
		{
			assert(rhs._head == _head && "invalid Iterator");

			if (_node == rhs._node)
				return false;

			Iterator it = *this;

			while (it._node != _head)
			{
				if (it._node->_next == rhs._node)
					return true;
				++it;
			}

			return false;
		}

		bool operator<=(const Iterator& rhs) const
		{
			if (_node == rhs._node)
				return true;

			return *this < rhs;
		}

		Node<T>* _node;
		Node<T>* _head;
	};

	List()
	{
		_head = std::allocator<Node<T>>().allocate(1);
		_head->_next = _head;
		_head->_prev = _head;
	}

	~List()
	{
		clear();
		std::allocator<Node<T>>().deallocate(_head, 1);
	}

	void push_back(T&& data)
	{
		Node<T>* newNode = new Node<T>(std::forward<T>(data));

		_head->_prev->_next = newNode;
		newNode->_prev = _head->_prev;
		newNode->_next = _head;
		_head->_prev = newNode;

		_size++;
	}

	template <typename... Args>
	T& emplace_back(Args&&... args)
	{
		Node<T>* newNode = new Node<T>(std::forward<Args>(args)...);

		_head->_prev->_next = newNode;
		newNode->_prev = _head->_prev;
		newNode->_next = _head;
		_head->_prev = newNode;

		return newNode->_data;
	}

	Iterator insert(const Iterator& pos, const T& value)
	{
		assert(pos._head == _head && "invalid Iterator");

		Node<T>* newNode = new Node<T>(std::move(value));

		newNode->_next = pos._node;
		newNode->_prev = pos._node->_prev;

		pos._node->_prev->_next = newNode;
		pos._node->_prev = newNode;

		_size++;

		return Iterator{ newNode, _head };
	}

	Iterator insert(const Iterator& pos, T&& value)
	{
		assert(pos._head == _head && "invalid Iterator");

		Node<T>* newNode = new Node<T>(std::forward<T>(value));

		newNode->_next = pos._node;
		newNode->_prev = pos._node->_prev;

		pos._node->_prev->_next = newNode;
		pos._node->_prev = newNode;

		_size++;

		return Iterator{ newNode, _head };
	}

	Iterator insert(const Iterator& pos, size_t count, const T& value)
	{
		assert(pos._head == _head && "invalid Iterator");

		for (size_t i = 0; i < count; ++i)
		{
			insert(pos, value);
		}

		return Iterator{ pos._node->_next, _head };
	}

	Iterator erase(const Iterator& pos)
	{
		assert(pos._head == _head && "invalid Iterator");

		if (pos._node == _head)
			return end();

		pos._node->_prev->_next = pos._node->_next;
		pos._node->_next->_prev = pos._node->_prev;

		Iterator next{ pos._node->_next, _head };

		delete pos._node;
		_size--;

		return next;
	}

	Iterator erase(const Iterator& first, const Iterator& last)
	{
		assert(first._head == _head && last._head == _head && "invalid Iterator");

		if (last <= first)
			return last;

		Iterator current = first;

		while (current != last)
		{
			current = erase(current);
		}

		return last;
	}

	void clear()
	{
		Node<T>* current = _head->_next;

		while (current != _head)
		{
			Node<T>* next = current->_next;
			delete current;
			current = next;
		}

		_head->_next = _head;
		_head->_prev = _head;

		_size = 0;
	}

	bool empty() const
	{
		return _size == 0;
	}

	size_t size() const
	{
		return _size;
	}

	Iterator begin() const
	{
		return Iterator{ _head->_next, _head };
	}

	Iterator end() const
	{
		return Iterator{ _head, _head };
	}

private:
	Node<T>* _head = nullptr;
	size_t _size = 0;
};