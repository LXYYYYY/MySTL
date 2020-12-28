#ifndef ITERATOR_H
#define ITERATOR_H

// iteratorͷ�ļ����ڵ�����,������ģ��ṹ���ȫ�ֺ���

//C++��׼���ͷ�ļ�,size_t ���Ͷ�����cstddefͷ�ļ���
#include <cstddef>

#include "type_traits.h"

namespace mystl
{
	// ���ֵ���������
	struct input_iterator_tag {};												// ���ֵ�������ָ�����������ı䣬ֻ��
	struct output_iterator_tag {};												// ֻд
	struct forward_iterator_tag : public input_iterator_tag {};					// �����д
	struct bidirectional_iterator_tag : public forward_iterator_tag {};			// ����˫���ƶ�
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};	//��������ָ���������� ����+ - [] <

	// iterator ģ��
	template<class Category, class T, class Distance = ptrdiff_t,
		class Pointer = T*, class Reference = T&>
		struct iterator
	{
		typedef Category	iterator_category;	//��ʾ����������ִ�еĲ��� ��Ӧ������������������
		typedef T			value_type;			//������������ָ��������		 	
		typedef Pointer		pointer;			//��ʾ������ָ�����
		typedef	Reference	reference;			//��ʾ������������� 
		typedef Distance	difference_type;	//��ʾ����������֮��ľ��룬���Ҳ����������ʾһ���������������
	};
	// iterator traits
	template <class T>
	struct has_iterator_cat
	{
	private:
		struct two { char a; char b; };
		template <class U> static two test(...);
		template <class U> static char test(typename U::iterator_category* = 0);
	public:
		static const bool value = sizeof(test<T>(0)) == sizeof(char);
	};

	template <class Iterator, bool>
	struct iterator_traits_impl{};

	template <class Iterator>
	struct iterator_traits_impl<Iterator, true>
	{
		typedef typename Iterator::iterator_category iterator_category;
		typedef typename Iterator::value_type        value_type;
		typedef typename Iterator::pointer           pointer;
		typedef typename Iterator::reference         reference;
		typedef typename Iterator::difference_type   difference_type;
	};

	template <class Iterator, bool>
	struct iterator_traits_helper {};

	template <class Iterator>
	struct iterator_traits_helper<Iterator, true>
		: public iterator_traits_impl<Iterator,
		std::is_convertible<typename Iterator::iterator_category, input_iterator_tag>::value ||
		std::is_convertible<typename Iterator::iterator_category, output_iterator_tag>::value>
	{
	};

	// ��ȡ������������
	template <class Iterator>
	struct iterator_traits
		: public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value>{};
	
	// ���ԭ��ָ���ƫ�ػ���
	template <class T>
	struct iterator_traits<T*> 
	{
		typedef random_access_iterator_tag	iterator_categoty;
		typedef T							value_type;
		typedef prtdiff_t					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};

	//���ԭ��֮pointer-to-const��ƫ�ػ���
	template <class T> 
	struct iterator_traits<const T*>
	{
		typedef random_access_iterator_tag	iterator_categoty;
		typedef T							value_type;
		typedef prtdiff_t					difference_type;
		typedef T* pointer;
		typedef T& reference;
	};

	template <class T, class U, bool = has_iterator_cat<iterator_traits<T>>::value>
	struct has_iterator_cat_of
		: public m_bool_constant<std::is_convertible<
		typename iterator_traits<T>::iterator_category, U>::value>
	{
	};

	// ��ȡĳ�ֵ�����
	template <class T, class U>
	struct has_iterator_cat_of<T, U, false> : public m_false_type {};

	template <class Iter>
	struct is_input_iterator : public has_iterator_cat_of<Iter, input_iterator_tag> {};

	template <class Iter>
	struct is_output_iterator : public has_iterator_cat_of<Iter, output_iterator_tag> {};

	template <class Iter>
	struct is_forward_iterator : public has_iterator_cat_of<Iter, forward_iterator_tag> {};

	template <class Iter>
	struct is_bidirectional_iterator : public has_iterator_cat_of<Iter, bidirectional_iterator_tag> {};

	template <class Iter>
	struct is_random_access_iterator : public has_iterator_cat_of<Iter, random_access_iterator_tag> {};

	template <class Iterator>
	struct is_iterator :
		public m_bool_constant<is_input_iterator<Iterator>::value ||
		is_output_iterator<Iterator>::value>
	{
	};

	// ��ȡĳ���������� category
	template <class Iterator>
	typename iterator_traits<Iterator>::iterator_category
		iterator_category(const Iterator&)
	{
		typedef typename iterator_traits<Iterator>::iterator_category Category;
		return Category();
	}

	// ��ȡĳ���������� distance_type
	template <class Iterator>
	typename iterator_traits<Iterator>::difference_type*
		distance_type(const Iterator&)
	{
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}

	// ��ȡĳ���������� value_type
	template <class Iterator>
	typename iterator_traits<Iterator>::value_type*
		value_type(const Iterator&)
	{
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}

	// ����������distance����
	template <class InputIterator>
	typename iterator_traits<InputIterator>::difference_type
		distance_dispatch(InputIterator first, InputIterator last, input_iterator_tag)
	{
		typename iterator_traits<InputIterator>::difference_type n = 0;
		while (first != last)
		{
			++first;
			++n;
		}
		return n;
	}

	// distance �� random_access_iterator_tag �İ汾
	template <class RandomIter>
	typename iterator_traits<RandomIter>::difference_type
		distance_dispatch(RandomIter first, RandomIter last,
			random_access_iterator_tag)
	{
		return last - first;
	}

	template <class InputIterator>
	typename iterator_traits<InputIterator>::difference_type
		distance(InputIterator first, InputIterator last)
	{
		return distance_dispatch(first, last, iterator_category(first));
	}

	// ���º��������õ�����ǰ�� n ������

	// advance �� input_iterator_tag �İ汾
	template <class InputIterator, class Distance>
	void advance_dispatch(InputIterator& i, Distance n, input_iterator_tag)
	{
		while (n--)
			++i;
	}

	// advance �� bidirectional_iterator_tag �İ汾
	template <class BidirectionalIterator, class Distance>
	void advance_dispatch(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag)
	{
		if (n >= 0)
			while (n--)  ++i;
		else
			while (n++)  --i;
	}

	// advance �� random_access_iterator_tag �İ汾
	template <class RandomIter, class Distance>
	void advance_dispatch(RandomIter& i, Distance n, random_access_iterator_tag)
	{
		i += n;
	}

	template <class InputIterator, class Distance>
	void advance(InputIterator& i, Distance n)
	{
		advance_dispatch(i, n, iterator_category(i));
	}
}

#endif // !ITERATOR_H
