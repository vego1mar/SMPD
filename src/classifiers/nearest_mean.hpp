#ifndef NEAREST_MEAN_HPP
#define NEAREST_MEAN_HPP


namespace classifiers {

    class NearestMean {
    public:
        NearestMean() = default;

        NearestMean(const NearestMean &rhs) = delete;

        NearestMean(NearestMean &&rvalue) noexcept = delete;

        NearestMean &operator=(const NearestMean &rhs) = delete;

        NearestMean &operator=(NearestMean &&rvalue) noexcept = delete;

        virtual ~NearestMean() = default;

    };

}

#endif //NEAREST_MEAN_HPP
