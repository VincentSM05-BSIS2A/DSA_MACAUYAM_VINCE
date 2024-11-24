#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;

// Function prototypes
sf::Text createText(const string& content, const sf::Font& font, float x, float y, unsigned int size = 24);
sf::Sprite loadBackground(const string& filePath, sf::Texture& texture, float scaleX = 1.0f, float scaleY = 1.0f);
void displayIntroScreen(const sf::Font& font, const sf::Sprite& background);
void insertShipment(std::vector<int>& heap, std::vector<int>& availableShipments, const sf::Font& font, const sf::Sprite& background);
void dispatchShipment(std::vector<int>& heap, const sf::Font& font, const sf::Sprite& background);
void viewHeap(const std::vector<int>& heap, const sf::Font& font, const sf::Sprite& background);
void heapify(vector<int>& heap, int n, int i);
void buildHeap(vector<int>& heap);

// Function to create text for displaying in the window
sf::Text createText(const string& content, const sf::Font& font, float x, float y, unsigned int size) {
    sf::Text text;
    text.setFont(font);
    text.setString(content);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::Green);
    text.setPosition(x, y);
    return text;
}

// Function to load and scale a background image
sf::Sprite loadBackground(const string& filePath, sf::Texture& texture, float scaleX, float scaleY) {
    if (!texture.loadFromFile(filePath)) {
        cout << "Error: Could not load " << filePath << endl;
    }
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setScale(scaleX, scaleY);
    return sprite;
}

// Function to maintain heap property by "heapifying" from a node downwards
void heapify(vector<int>& heap, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && heap[left] > heap[largest])
        largest = left;

    if (right < n && heap[right] > heap[largest])
        largest = right;

    if (largest != i) {
        swap(heap[i], heap[largest]);
        heapify(heap, n, largest);
    }
}

// Function to build a max-heap from the array
void buildHeap(vector<int>& heap) {
    int n = heap.size();
    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(heap, n, i);
    }
}

// Function to display the intro screen
void displayIntroScreen(const sf::Font& font, const sf::Sprite& background) {
    sf::RenderWindow introWindow(sf::VideoMode(785, 600), "Galactic Cargo Management - Intro");

    // Initial text size
    unsigned int textSize = 10;

    // Backstory and task text
    string content =
        "\n\n\n\nBackstory:\n\nIn the far future, humans have built colonies on planets across the galaxy.\n\n"
        "These colonies depend on shipments of important supplies like food, medicine,\n\n"
        "and tools. You are the cargo manager on the SS Nexus. Your job is to prioritize\n\n"
        "and deliver shipments efficiently to keep the colonies thriving.\n\n\n\n\n\n"
        "Mission:\n\n"
        "1. Insert Shipments: Add new shipments without disrupting priority.\n\n"
        "2. Dispatch Shipments: Deliver the most urgent shipments.\n\n"
        "3. Maintain Order: Use the Heapify process to keep the system organized.\n\n"
        "4. Monitor Changes: Track how actions impact priorities.\n\n"
        "Use UP and DOWN arrow keys to adjust text size.\n\n"
        "\n\n\n\n\n\n\n\nPress SPACE to begin...";

    sf::Text introText = createText(content, font, 30, 30, textSize);

    while (introWindow.isOpen()) {
        sf::Event event;
        while (introWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                introWindow.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    introWindow.close(); // Close intro screen
                }
                if (event.key.code == sf::Keyboard::Up) {
                    textSize = min(textSize + 2, 40U); // Increase text size, cap at 40
                }
                if (event.key.code == sf::Keyboard::Down) {
                    textSize = max(textSize - 2, 12U); // Decrease text size, floor at 12
                }
                introText.setCharacterSize(textSize); // Update text size
            }
        }

        introWindow.clear();
        introWindow.draw(background);
        introWindow.draw(introText);
        introWindow.display();
    }
}

// Function to insert a new shipment
void insertShipment(vector<int>& heap, vector<int>& availableShipments, const sf::Font& font, const sf::Sprite& background) {
    sf::RenderWindow window(sf::VideoMode(785, 600), "Insert Shipment");
    sf::String userInput;
    sf::Text instructions = createText("\nEnter shipment value to insert: ", font, 50, 50);
    sf::Text availableText = createText("\nAvailable Shipments: ", font, 50, 100);
    sf::Text inputText = createText("", font, 50, 200);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !userInput.isEmpty())
                    userInput.erase(userInput.getSize() - 1);
                else if (event.text.unicode == '\r') {
                    string input = userInput.toAnsiString();
                    try {
                        int value = stoi(input);
                        auto it = find(availableShipments.begin(), availableShipments.end(), value);
                        if (it != availableShipments.end()) {
                            heap.push_back(value);
                            buildHeap(heap);
                            availableShipments.erase(it);
                        }
                    }
                    catch (...) {
                        cout << "Invalid input. Please try again." << endl;
                    }
                    userInput.clear();
                }
                else if (event.text.unicode < 128) {
                    userInput += event.text.unicode;
                }
            }
        }

        stringstream ss;
        ss << "\n\n\n\nAvailable Shipments: ";
        for (int val : availableShipments) ss << val << " ";
        availableText.setString(ss.str());
        inputText.setString("\n\n\n\nInput: " + userInput.toAnsiString());

        window.clear();
        window.draw(background);
        window.draw(instructions);
        window.draw(availableText);
        window.draw(inputText);
        window.display();
    }
}

// Function to dispatch the highest-priority shipment
void dispatchShipment(vector<int>& heap, const sf::Font& font, const sf::Sprite& background) {
    sf::RenderWindow window(sf::VideoMode(785, 600), "Dispatch Shipment");

    string message;
    if (!heap.empty()) {
        int top = heap.front();
        heap[0] = heap.back();
        heap.pop_back();
        buildHeap(heap);
        message = "Dispatched shipment with priority:\n                             " + to_string(top);
    }
    else {
        message = "No shipments to dispatch!";
    }

    sf::Text messageText = createText(message, font, 50, 250);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(background);
        window.draw(messageText);
        window.display();
    }
}

// Function to view the current heap
void viewHeap(const vector<int>& heap, const sf::Font& font, const sf::Sprite& background) {
    sf::RenderWindow window(sf::VideoMode(785, 600), "View Heap");

    stringstream ss;
    ss << "\n\nCurrent Heap: ";
    for (int val : heap) ss << val << " ";

    sf::Text heapText = createText(ss.str(), font, 50, 200);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(background);
        window.draw(heapText);
        window.display();
    }
}

int main() {
    vector<int> heap;
    vector<int> availableShipments = { 12, 35, 9, 18, 23 };

    sf::RenderWindow window(sf::VideoMode(785, 600), "Cargo Management");
    sf::Font font;
    if (!font.loadFromFile("SPACEBOY.ttf")) {
        cout << "Error loading font!" << endl;
        return -1;
    }

    sf::Texture introTexture, menuTexture, insertTexture, dispatchTexture, viewTexture;
    sf::Sprite introBackground = loadBackground("intro.jpg", introTexture, 0.5f, 0.6f);
    sf::Sprite menuBackground = loadBackground("startup.jpg", menuTexture, 0.5f, 0.6f); // Adjust the scale to fit your window size
    sf::Sprite insertBackground = loadBackground("insert.png", insertTexture, 1.7f, 1.5f);
    sf::Sprite dispatchBackground = loadBackground("dispatch.png", dispatchTexture, 1.6f, 1.6f);
    sf::Sprite viewBackground = loadBackground("view.png", viewTexture, 1.9f, 1.9f);

    // Display the introduction screen first
    displayIntroScreen(font, introBackground);

    sf::Text menuText = createText(
        "1. Insert Shipment\n\n"
        "2. Dispatch Shipment\n\n"
        "3. View Heap ",
        font, 350, 150);
    sf::String userInput;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !userInput.isEmpty())
                    userInput.erase(userInput.getSize() - 1);
                else if (event.text.unicode == '\r') {
                    string input = userInput.toAnsiString();
                    if (input == "1") {
                        insertShipment(heap, availableShipments, font, insertBackground);
                    }
                    else if (input == "2") {
                        dispatchShipment(heap, font, dispatchBackground);
                    }
                    else if (input == "3") {
                        viewHeap(heap, font, viewBackground);
                    }
                    userInput.clear();
                }
                else if (event.text.unicode < 128) {
                    userInput += event.text.unicode;
                }
            }
        }

        sf::Text inputText = createText("Enter number: " + userInput.toAnsiString(), font, 350, 500);

        window.clear();
        window.draw(menuBackground); // Use the menu background here
        window.draw(menuText);
        window.draw(inputText);
        window.display();
    }

    return 0;
}
