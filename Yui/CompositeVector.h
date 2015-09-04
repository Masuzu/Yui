#ifndef __COMPOSITE_VECTOR_H__
#define __COMPOSITE_VECTOR_H__

#include <vector>

namespace Yui
{
    // A class to a collection of vectors (the references alone are kept) and provides an iterator to iterate through all of them
    // in the same order they were added. Motivated by the needs of cross validation in which the data is split in multiple bags 
    // where often a single bag is used for validation while the others are used for training. 
    template<class T>
    class CompositeVector
    {
    private:
        std::vector<std::vector<T>*> vectors_;
        
    public:
        const std::vector<T> *&at(size_t i)    const    {return vectors_[i];}
        std::vector<T> *&at(size_t i)    {return vectors_[i];}
        const std::vector<T> *&operator[](size_t i)    const    {return vectors_[i];}
        std::vector<T> *&operator[](size_t i)    {return vectors_[i];}
        void PushBack(const std::vector<T> &v)	{vectors_.push_back(const_cast<std::vector<T>*>(&v));}
        // Number of vectors held
        inline size_t num_vectors()    {return vectors_.size();}
        
        // Check http://stackoverflow.com/questions/6438086/iterator-invalidation-rules for iterator invalidation rules
        class Iterator : public std::iterator<std::input_iterator_tag, T>
        {
        	friend class CompositeVector<T>;
        private:
            CompositeVector<T> *collection_;
            typedef typename std::vector<T>::iterator VectorIterator;
            VectorIterator current_position_in_vector_;
            typedef typename std::vector<std::vector<T>*>::iterator VectorVectorIterator;
            VectorVectorIterator current_vector_;
            
            Iterator()  {};
            
            void IncrementIteratorsUntilFirstDefinedElement()
            {
            	// Find the first defined element
    	        std::vector<T> *vector = *current_vector_;
    	        while(current_position_in_vector_ == vector->end())
    	        {
    	        	++current_vector_;
    	            if(current_vector_ != collection_->vectors_.end())
    	            {
    	                vector = *current_vector_;
    	                current_position_in_vector_ = vector->begin();
    	            }
    	            else	// No more elements
    	            	break;
    	        }
            }
            
        public:
                
            Iterator(const Iterator& mit) :
                collection_(mit.collection_),
                current_position_in_vector_(mit.current_position_in_vector_),
                current_vector_(mit.current_vector_)
            {}
            
            Iterator& operator++()
            {
                std::vector<T> *vector = *current_vector_;
                if(current_position_in_vector_ == vector->end())
                {
                	if(current_vector_ != collection_->vectors_.end())
                    {
                        ++current_vector_;
                        vector = *current_vector_;
                        current_position_in_vector_ = vector->begin();
                        IncrementIteratorsUntilFirstDefinedElement();
                    }
                }
                else
                {
                	++current_position_in_vector_;
                    IncrementIteratorsUntilFirstDefinedElement();
                }
                return *this;
            }
        
            bool operator==(const Iterator& rhs)	const
            {
                if(current_vector_ != collection_->vectors_.end() && rhs.current_vector_ != rhs.collection_->vectors_.end())
                {
                    return collection_ == rhs.collection_
                    && current_vector_ == rhs.current_vector_
                    && current_position_in_vector_ == rhs.current_position_in_vector_;
                }
                // Case where both iterators are pointing at the end of the collection
                else if(current_vector_ == collection_->vectors_.end() && rhs.current_vector_ == rhs.collection_->vectors_.end())
                {
                    return collection_ == rhs.collection_
                    && current_vector_ == rhs.current_vector_;
                }
                else
                    return false;
            }
            bool operator!=(const Iterator& rhs)	const
            {
                return !(*this==rhs);
            }
            T& operator*()
            {
                return *current_position_in_vector_;
            }
        };
        
        Iterator begin()
        {
            Iterator it;
            it.collection_ = this;
            it.current_vector_ = vectors_.begin();
            if(!vectors_.empty())
                it.current_position_in_vector_ = vectors_[0]->begin();
            // Find the first defined element
            it.IncrementIteratorsUntilFirstDefinedElement();
            return it;
        }
        
        Iterator end()
        {
            Iterator it;
            it.collection_ = this;
            it.current_vector_ = vectors_.end();
            return it;
        }
    };
}

#endif

/*
#include <iostream>

int main() {
	CompositeVector<int> test;
	std::vector<int> v1;
	//v1.push_back(1);
	//v1.push_back(2);
	std::vector<int> v2;
	v1.push_back(3);
	v1.push_back(4);
	test.PushBack(v1);
	test.PushBack(v2);
	auto it = test.begin();
	for(; it != test.end(); ++it)
		std::cout << *it << std::endl;
	std::cout << (test.end() == it) << std::endl;
	return 0;
}
*/
