#include <iostream>
#include <deque>
#include <fstream>
#include <string>
#include <random>

class Node
{
public:
    int id;
    std::string text;
};

class NodeDeque
{
private:
    std::deque<Node> nodeDeque;
    int nodeCounter = 0;
    int pagingCounter = 0;
    const int maxNodesInMemory = 5000;
    const int nodesPerPage = 1000;
    const int maxNodesInFile = 1000;

public:
    void addNode(Node node)
    {
        if (nodeDeque.size() == maxNodesInMemory)
        {
            saveNodesToHDD();
            pagingCounter++;
        }

        nodeDeque.push_back(node);

        if (nodeDeque.size() > maxNodesInMemory)
        {
            saveNodeToFile(nodeDeque.front());
            nodeDeque.pop_front();
        }
    }

    void saveNodesToHDD()
    {
        for (int i = 0; i < nodesPerPage; i++)
        {
            saveNodeToFile(nodeDeque.front());
            nodeDeque.pop_front();
        }
    }

  void saveNodeToFile(Node node) {
    int startRange = (nodeCounter / maxNodesInFile) * maxNodesInFile;
    std::string fileName = std::to_string(startRange) + "_" + std::to_string(startRange + maxNodesInFile - 1) + ".dat";
    static std::vector<std::string> lines;
    lines.push_back(std::to_string(node.id) + " " + node.text);

    nodeCounter++;

    if (nodeCounter % maxNodesInFile == 0) {
        std::ofstream file;
        file.open(fileName);
        for (int i = lines.size() - 1; i >= 0; i--) {
            file << lines[i] << '\n';
        }
        file.close();
        lines.clear();
    }
}


    void traverse() {
        // First, print all nodes that are currently in the deque
        for (int i = nodeDeque.size() - 1; i >= 0; i--)
        {
            std::cout << nodeDeque[i].id << " " << nodeDeque[i].text << '\n';
        }

        // Then, read the nodes from the disk
        for (int i = 0; i < pagingCounter; i++)
        {
            int startRange = i * nodesPerPage;
            int endRange = startRange + nodesPerPage - 1;
            std::ifstream file;
            file.open(std::to_string(startRange) + "_" + std::to_string(endRange) + ".dat");
            if (file.is_open())
            {
                int id;
                std::string text;
                while (file >> id >> text)
                {
                    std::cout << id << " " << text << " read from file\n";
                }
                file.close();
            }
        }
    }

    void readNodesFromFile() {
        int startRange = nodeCounter - maxNodesInMemory;
        std::ifstream file;
        file.open(std::to_string(startRange) + "_" + std::to_string(startRange + nodesPerPage - 1) + ".dat");
        if (file.is_open())
        {
            int id;
            std::string text;
            while (file >> id >> text)
            {
                std::cout << id << " " << text << " read from file\n";
            }
            file.close();
        }
    }
};

int main() {
    NodeDeque nodeDeque;

    for (int i = 0; i < 6000; i++)
    {
        Node node;
        node.id = i;
        node.text = "text" + std::to_string(i);
        nodeDeque.addNode(node);
    }

    nodeDeque.traverse();

    return 0;
}
