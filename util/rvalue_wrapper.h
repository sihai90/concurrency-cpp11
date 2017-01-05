/**
 * rvalue_wrapper.h
 */
#ifndef UTIL_RVALUE_WRAPPER_H_
#define UTIL_RVALUE_WRAPPER_H_

/**
 * A wrapper for holding and passing rvalues through std::bind. The rvalue wrapped will be stored
 * inside the wrapper until the functor returned by std::bind is invoked, at when the stored rvalue
 * will be moved from. The functor can be invoked ONLY ONCE.
 */
template<typename T>
struct RValueWrapper {
public:
    template<typename = typename std::enable_if<std::is_rvalue_reference<T&&>::value>::type>
    explicit RValueWrapper(T &&t) noexcept :
    t(std::move(t)) {
    }
    RValueWrapper(const RValueWrapper &rhs) = default;
    RValueWrapper(RValueWrapper &&rhs) = default;
    RValueWrapper& operator=(const RValueWrapper &rhs) = default;
    RValueWrapper& operator=(RValueWrapper &&rhs) = default;

    template <class ...Args>
    T &&operator()(Args ...) {
        return std::move(t);
    }
private:
    T t;
};

namespace std {
template<typename T>
struct is_bind_expression<RValueWrapper<T>> : std::true_type {
};
}

template<typename T,
        typename = typename std::enable_if<std::is_rvalue_reference<T&&>::value>::type>
RValueWrapper<T> rval(T &&t) {
    return RValueWrapper<T>(std::move(t));
}

template<typename T>
RValueWrapper<T> rval(T &t) {
    return RValueWrapper<T>(T(t));
}

#endif /* UTIL_RVALUE_WRAPPER_H_ */
