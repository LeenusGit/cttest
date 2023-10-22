#pragma once

#include "testresult.h"

#include <concepts>
#include <string>

namespace cttest
{

template <typename F, typename T>
concept hasUnaryOpMsg = requires (F op, const T& a, TestResult& res) {
    { op.msg(a, res) } -> std::convertible_to<std::string>;
};

template <typename F, typename T>
concept hasBinOpMsg = requires (F op, const T& a, const T& b, TestResult& res) {
    { op.msg(a, b, res) } -> std::convertible_to<std::string>;
};

template <typename Func>
concept TestFunc = requires (Func func) {
    { func() } -> std::convertible_to<TestResult>;
};

}