#include "Account.h"
#include "Transaction.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using testing::_;

class AccountMock : public Account {
public:
    explicit AccountMock(int id, int balance) : Account(id, balance) {}
    MOCK_CONST_METHOD0(GetBalance, int());
    MOCK_METHOD1(ChangeBalance, void(int diff));
    MOCK_METHOD0(Lock, void());
    MOCK_METHOD0(Unlock, void());
}; // class AccountMock

class TransactionMock : public Transaction {
public:
    MOCK_METHOD3(Make, bool(AccountMock& from, AccountMock& to, int sum));
}; // class TransactionMock

TEST(Transaction, Mock) {
    TransactionMock tr;
    AccountMock acc1(1, 100);
    AccountMock acc2(2, 300);

    EXPECT_CALL(tr, Make(_, _, _)).Times(5);
    tr.set_fee(200);
    tr.Make(acc1, acc2, 200);
    tr.Make(acc2, acc1, 300);
    // throws
    tr.Make(acc2, acc1, 50);
    tr.Make(acc2, acc1, 0);
    tr.Make(acc1, acc2, -8);
}

TEST(Transaction, TestTransaction) {
    Transaction tr;
    AccountMock a1(1, 100), a2(2, 300);
    tr.set_fee(10);
    EXPECT_EQ(tr.fee(), 10);
    EXPECT_THROW(tr.Make(a1, a2, 90), std::logic_error);
    EXPECT_THROW(tr.Make(a1, a2, -1), std::invalid_argument);
    EXPECT_THROW(tr.Make(a1, a1, 100), std::logic_error);
    EXPECT_FALSE(tr.Make(a1, a2, 400));
    EXPECT_FALSE(tr.Make(a2, a1, 300));
    EXPECT_FALSE(tr.Make(a2, a1, 246));
}
