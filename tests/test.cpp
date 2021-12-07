// Copyright 2021 byteihq <kotov038@gmail.com>

#include <gtest/gtest.h>
#include <SharedPtr.h>

TEST(SharedPtr, Init) {
    {
        SharedPtr<int> sp;
        EXPECT_EQ(sp.use_count(), 0);
        EXPECT_EQ(sp.get(), nullptr);
        EXPECT_FALSE(sp);
    }
    {
        SharedPtr<int> sp(new int(10));
        SharedPtr<int> sp1(std::move(sp));
        EXPECT_EQ(sp.get(), nullptr);
        EXPECT_EQ(sp.use_count(), 0);
    }
}

TEST(SharedPtr, InitwithParams) {
    {
        SharedPtr<size_t> sp(new size_t(10));
        EXPECT_TRUE(sp);
        EXPECT_EQ(sp.use_count(), 1);
        EXPECT_EQ(*sp, 10);
        EXPECT_EQ(*sp.get(), 10);
    }
    {
        SharedPtr<double> sp(new double(100.3));
        EXPECT_TRUE(sp);
        EXPECT_EQ(sp.use_count(), 1);
        EXPECT_EQ(*sp, 100.3);
        EXPECT_EQ(*sp.get(), 100.3);
    }
}

TEST(SharedPtr, Copy) {
    SharedPtr<float> sp1(new float(1.23));
    EXPECT_EQ(sp1.use_count(), 1);
    {
        SharedPtr<float> sp2(sp1); //sp2 -2 sp1 - 2
        EXPECT_EQ(sp1.use_count(), 2);
        EXPECT_EQ(sp2.use_count(), 2);
        EXPECT_EQ(sp1.get(), sp2.get());
        sp1 = sp2;
        EXPECT_EQ(sp1.use_count(), 2);
        EXPECT_EQ(sp2.use_count(), 2);
        SharedPtr<float> sp3(sp2); //
        sp1 = sp3;
        EXPECT_EQ(sp1.get(), sp3.get());
        EXPECT_EQ(sp2.get(), sp3.get());
        EXPECT_EQ(sp1.use_count(), 3);
        EXPECT_EQ(sp2.use_count(), 3);
        EXPECT_EQ(sp3.use_count(), 3);
    }
    EXPECT_EQ(sp1.use_count(), 1);
}

TEST(SharedPtr, Move) {
    SharedPtr<char> sp1(new char('c'));
    EXPECT_EQ(sp1.use_count(), 1);
    {
        SharedPtr<char> sp2 = std::move(sp1);
        EXPECT_FALSE(sp1);
        EXPECT_EQ(sp2.use_count(), 1);
        EXPECT_EQ(*sp2, 'c');
    }
}

TEST(SharedPtr, Reset) {
    SharedPtr<uint8_t> sp(new uint8_t(33));
    sp.reset();
    EXPECT_EQ(sp.use_count(), 0);
    EXPECT_EQ(sp.get(), nullptr);
    EXPECT_FALSE(sp);
    sp.reset(); //try to reset an empty ptr
    EXPECT_EQ(sp.use_count(), 0);
    EXPECT_EQ(sp.get(), nullptr);
    EXPECT_FALSE(sp);
    sp.reset(new uint8_t(14));
    EXPECT_EQ(sp.use_count(), 1);
    EXPECT_EQ(*sp, 14);
    EXPECT_TRUE(sp);
}

TEST(SharedPtr, Swap) {
    SharedPtr<int> sp1(new int(1));
    int* ptr1 = sp1.get();
    SharedPtr<int> sp2(new int(2));
    int* ptr2 = sp2.get();
    sp1.swap(sp2);
    EXPECT_EQ(sp1.use_count(), 1);
    EXPECT_EQ(sp1.get(), ptr2);
    EXPECT_EQ(*sp1, 2);
    EXPECT_EQ(sp2.use_count(), 1);
    EXPECT_EQ(sp2.get(), ptr1);
    EXPECT_EQ(*sp2, 1);
}
