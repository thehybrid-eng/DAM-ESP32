#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

class NetworkManager {
public:
    void begin();

    void update();

    bool isConnected();
};

extern NetworkManager network;

#endif