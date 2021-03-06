#include <gtest/gtest.h>

#include <QDebug>

#include "track/bpm.h"

class BpmTest : public testing::Test {
};

TEST_F(BpmTest, defaultCtor) {
    EXPECT_EQ(mixxx::Bpm{mixxx::Bpm::kValueUndefined}, mixxx::Bpm{});
}

TEST_F(BpmTest, isValid) {
    EXPECT_FALSE(mixxx::Bpm{mixxx::Bpm::kValueUndefined}.isValid());
    EXPECT_FALSE(mixxx::Bpm{mixxx::Bpm::kValueMin}.isValid());
    EXPECT_FALSE(mixxx::Bpm{-mixxx::Bpm::kValueMin}.isValid());
    EXPECT_FALSE(mixxx::Bpm{mixxx::Bpm::kValueMin - 0.001}.isValid());
    EXPECT_TRUE(mixxx::Bpm{mixxx::Bpm::kValueMin + 0.001}.isValid());
    EXPECT_TRUE(mixxx::Bpm{mixxx::Bpm::kValueMax}.isValid());
    EXPECT_FALSE(mixxx::Bpm{-mixxx::Bpm::kValueMax}.isValid());
    EXPECT_TRUE(mixxx::Bpm{mixxx::Bpm::kValueMax - 0.001}.isValid());
    // The upper bound is only a soft-limit!
    EXPECT_TRUE(mixxx::Bpm{mixxx::Bpm::kValueMax + 0.001}.isValid());
    EXPECT_TRUE(mixxx::Bpm{123.45}.isValid());
    EXPECT_FALSE(mixxx::Bpm{-123.45}.isValid());
}

TEST_F(BpmTest, value) {
    EXPECT_DOUBLE_EQ(123.45, mixxx::Bpm{123.45}.value());
    EXPECT_DOUBLE_EQ(mixxx::Bpm::kValueMin + 0.001,
            mixxx::Bpm{mixxx::Bpm::kValueMin + 0.001}.value());
    // The upper bound is only a soft-limit!
    EXPECT_DOUBLE_EQ(mixxx::Bpm::kValueMax + 0.001,
            mixxx::Bpm{mixxx::Bpm::kValueMax + 0.001}.value());
}

TEST_F(BpmTest, valueOr) {
    EXPECT_DOUBLE_EQ(123.45, mixxx::Bpm{123.45}.valueOr(-1.0));
    EXPECT_EQ(-1.0, mixxx::Bpm{-123.45}.valueOr(-1.0));
    EXPECT_EQ(123.45, mixxx::Bpm{}.valueOr(123.45));
    EXPECT_EQ(mixxx::Bpm::kValueUndefined,
            mixxx::Bpm{mixxx::Bpm::kValueMin}.valueOr(
                    mixxx::Bpm::kValueUndefined));
    EXPECT_DOUBLE_EQ(mixxx::Bpm::kValueMin + 0.001,
            mixxx::Bpm{mixxx::Bpm::kValueMin + 0.001}.value());
    EXPECT_EQ(mixxx::Bpm::kValueMax, mixxx::Bpm{mixxx::Bpm::kValueMax}.valueOr(100.0));
    // The upper bound is only a soft-limit!
    EXPECT_DOUBLE_EQ(mixxx::Bpm::kValueMax + 0.001,
            mixxx::Bpm{mixxx::Bpm::kValueMax + 0.001}.valueOr(mixxx::Bpm::kValueMax));
}

TEST_F(BpmTest, TestBpmComparisonOperators) {
    EXPECT_EQ(mixxx::Bpm(120), mixxx::Bpm(120));
    EXPECT_EQ(mixxx::Bpm(120), mixxx::Bpm(60) * 2);
    EXPECT_EQ(mixxx::Bpm(120), mixxx::Bpm(240) / 2);

    EXPECT_LT(mixxx::Bpm(120.0), mixxx::Bpm(130.0));
    EXPECT_LE(mixxx::Bpm(120.0), mixxx::Bpm(130.0));
    EXPECT_LE(mixxx::Bpm(120.0), mixxx::Bpm(120.0));

    EXPECT_GT(mixxx::Bpm(130.0), mixxx::Bpm(120.0));
    EXPECT_GE(mixxx::Bpm(130.0), mixxx::Bpm(120.0));
    EXPECT_GE(mixxx::Bpm(130.0), mixxx::Bpm(130.0));

    // Verify that invalid values are equal to each other, regardless of their
    // actual value.
    EXPECT_EQ(mixxx::Bpm(mixxx::Bpm::kValueUndefined), mixxx::Bpm());
    EXPECT_EQ(mixxx::Bpm(0.0), mixxx::Bpm());
    EXPECT_EQ(mixxx::Bpm(-120.0), mixxx::Bpm());
    EXPECT_EQ(mixxx::Bpm(-120.0), mixxx::Bpm(0.0));
    EXPECT_EQ(mixxx::Bpm(-120.0), mixxx::Bpm(-100.0));

    // Here, both values are invalid and therefore equal, so both <= and >= returns true.
    EXPECT_LE(mixxx::Bpm(-120.0), mixxx::Bpm(-100.0));
    EXPECT_GE(mixxx::Bpm(-120.0), mixxx::Bpm(-100.0));

    // Verify that valid and invalid values are not equal
    EXPECT_NE(mixxx::Bpm(120.0), mixxx::Bpm());
    EXPECT_NE(mixxx::Bpm(120.0), mixxx::Bpm(-120.0));
}
