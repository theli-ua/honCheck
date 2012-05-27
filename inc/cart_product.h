#ifndef _CART_PRODUCT_H
#define _CART_PRODUCT_H
#include <vector>

/**
 * @brief Struct to hold iterators while building product
 */
template <typename T>
struct Digits{
    typename std::vector<T>::const_iterator begin;
    typename std::vector<T>::const_iterator end;
    typename std::vector<T>::const_iterator me;
};

template <typename T>
/**
 * @brief Build a product
 *
 * @param out output vector
 * @param in input vector
 */
void cart_product(
        std::vector<std::vector<T> >& out,  // final result
        std::vector<std::vector<T> >& in)  // input

{
    if (in.size() == 0)
        return;
    typename std::vector<Digits<T> > vd;

    // Start all of the iterators at the beginning.
    for(typename std::vector<std::vector<T> >::const_iterator it = in.begin();
            it != in.end();
            ++it) {
        Digits<T> d = {(*it).begin(), (*it).end(), (*it).begin()};
        vd.push_back(d);
    }


    while(1) {

        // Construct your first product vector by pulling 
        // out the element of each vector std::vector<std::vector<T> >a the iterator.
        std::vector<T> result;
        for(typename std::vector<Digits<T> >::const_iterator it = vd.begin();
                it != vd.end();
                it++) {
            result.push_back(*(it->me));
        }
        out.push_back(result);

        // Increment the rightmost one, and repeat.

        // When you reach the end, reset that one to the beginning and
        // increment the next-to-last one. You can get the "next-to-last"
        // iterator by pulling it out of the neighboring element in your
        // vector of iterators.
        for(typename std::vector<Digits<T> >::iterator it = vd.begin(); ; ) {
            // okay, I started at the left instead. sue me
            ++(it->me);
            if(it->me == it->end) {
                if(it+1 == vd.end()) {
                    // I'm the last digit, and I'm about to roll
                    return;
                } else {
                    // cascade
                    it->me = it->begin;
                    ++it;
                }
            } else {
                // normal
                break;
            }
        }
    }
}
#endif
