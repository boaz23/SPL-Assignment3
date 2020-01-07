#ifndef STOMPCLIENT_PROTOCOL_H
#define STOMPCLIENT_PROTOCOL_H


template <typename Tin, typename Tout> class Protocol {
public:
    virtual Tout process(Tin msg) = 0;
    virtual bool shouldTerminate() = 0;
};


#endif //STOMPCLIENT_PROTOCOL_H
