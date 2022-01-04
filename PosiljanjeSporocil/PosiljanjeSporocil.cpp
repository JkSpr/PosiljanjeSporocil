// PosiljanjeSporocil.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <thread>
#include <algorithm>
#include <random>

class Message
{
public:
    struct timestamp
    {
        std::time_t created = NULL;
        std::time_t read = NULL;
    };
    timestamp timestamp;

    Message(std::string message)
    {
        SetContent(message);
    }

    Message() = default;

    void SetContent(std::string message)
    {
        timestamp.created = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        content = message;
    }
    std::string GetContent()
    {
        timestamp.read = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        return content;
    }

private:
    std::string content;
};

class MessageSender
{
public:
    std::vector<Message> message;

    void CreateMessagesFromFile()
    {
        std::string singleMessage;
        std::ifstream messagesFile ("D:/Privat/Naloga/PosiljanjeSporocil/messages.txt");

        if (messagesFile.is_open())
        {
            std::cout << "file is open!\n";
            int i = 0;
            std::cout << "\nCreating messages!\n";
            while (getline(messagesFile, singleMessage))
            {
                std::cout << singleMessage << "\n";
                message.push_back(Message(singleMessage));
                //std::cout << message[i].timestamp.created << "\n";
                i++;
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
            }
        }
        else
        {
            std::cout << "Message failed to open!\n";
        }
        messagesFile.close();
    }

    void ShufleMessagesByTimeStamp()
    {
        std::cout << "\nShuffling messages!\n";
        std::random_device rng_dev;
        std::mt19937 rng(rng_dev());
        //auto rng = std::default_random_engine{};
        std::shuffle(std::begin(message), std::end(message), rng);
        for (int i = 0; i < message.size(); i++)
        {
            std::cout << message[i].GetContent() << "\n";
        }
    }

};

class MessageReceiver
{
public:
    std::vector<Message> message;
    Message tempMessage;

    void SortMessagesByTimeStamp()
    {
        std::cout << "\nSorting messages!\n";
        bool change = true;
        while (change)
        {
            change = false;

            for (int i = 0; i < message.size() - 1; i++)
            {
                if (message[i].timestamp.created > message[i + 1].timestamp.created)
                {
                    tempMessage = message[i + 1];
                    message[i + 1] = message[i];
                    message[i] = tempMessage;
                    change = true;
                }
            }

        }
    }

    void PrintMessages()
    {
        for (int i = 0; i < message.size(); i++)
        {
            std::cout << message[i].GetContent() << "\n";
        }
    }

};

int main()
{
    std::cout << "Hello Communicator!\n";

    MessageSender messageSender;
    MessageReceiver messageReceiver;

    messageSender.CreateMessagesFromFile();
    messageSender.ShufleMessagesByTimeStamp();

    messageReceiver.message = messageSender.message;
    messageReceiver.SortMessagesByTimeStamp();
    messageReceiver.PrintMessages();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
