#include <gtest/gtest.h>

#include "src/client/Client.h"

TEST(Client, nick_and_socket_test) {
    Client client(10);
    client.setNick("nick1");

    ASSERT_EQ(client.getSocket(), 10);
    ASSERT_EQ(client.getNick(), "nick1");
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
