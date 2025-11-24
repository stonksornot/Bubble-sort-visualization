#include <iostream>
#include <vector>
#include <random>
#include <SFML/Graphics.hpp>

using namespace std;

int main()
{
    sf::ContextSettings settings;
    settings.majorVersion = 2;
    settings.minorVersion = 1;
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Sorting Visualizer");
    sf::Event event;


    sf::Font deja;
    if (!deja.loadFromFile("C:\\Users\\Rioca\\Documents\\Sorting_visual\\DejaVuSans.ttf")) {
        cerr << "Error: Font file not found or could not be loaded.\n" ;
        return EXIT_FAILURE;
    }
    sf::Text title;
    title.setFont(deja);
    title.setString("Sorting Visualizer");
    title.setCharacterSize(50);
    sf::FloatRect rect = title.getLocalBounds();
    title.setOrigin(rect.width / 2.0, rect.height / 2.0);
    title.setPosition(window.getSize().x / 2.0, 50);

    vector<int> arr(30);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 900);
    for(int i = 0; i < arr.size(); i++) {
        arr[i] = dis(gen);
    }


    int max_idx = 0;
    for(int i = 0; i < arr.size(); i++) {
        if(arr[i] > arr[max_idx]) {
            max_idx = i;
        }
    }

    const int NUM_BARS = arr.size();
    const double SCALE = 900.0/arr[max_idx];

    int i = 0, j = 0;
    int swaps = 0, comparisons = 0;
    double seconds = 0;
    bool sorted = false, swapped = false, lastswap = false;

    sf::Clock clock;

    while(window.isOpen()) {
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) window.close();
        }
        window.clear(sf::Color::Black);
        window.draw(title);

        double barWidth = (double)window.getSize().x / NUM_BARS;
        for(int k = 0; k < NUM_BARS; k++) {
            double height = arr[k] * SCALE;
            sf::RectangleShape bar(sf::Vector2f(barWidth - 1, height));
            bar.setPosition(k * barWidth, window.getSize().y - height - 50);

            if(!sorted && (k == j || k == j + 1)) {
                if(lastswap) {
                    bar.setFillColor(sf::Color::Red);
                } else {
                    bar.setFillColor(sf::Color::Yellow);
                }
            } else {
                bar.setFillColor(sf::Color::White);
            }
            if(sorted) {
                bar.setFillColor(sf::Color::Green);
            }
            
            window.draw(bar);
        }
        lastswap = false;

        sf::Text info_swaps;
        info_swaps.setFont(deja);
        info_swaps.setString("Swaps: " + to_string(swaps));
        info_swaps.setCharacterSize(20);
        info_swaps.setPosition(10, 50);

        sf::Text info_comparisons;
        info_comparisons.setFont(deja);
        info_comparisons.setString("Comparisons: " + to_string(comparisons));
        info_comparisons.setCharacterSize(20);
        info_comparisons.setPosition(10, 80);

        if(!sorted) {
            sf::Time elapsed = clock.getElapsedTime();
            seconds = elapsed.asSeconds();
        }
        sf::Text info_time;
        info_time.setFont(deja);
        info_time.setString("Time (not runtime): " + to_string(round(seconds*1000.0) / 1000.0) + " seconds");
        info_time.setCharacterSize(20);
        info_time.setPosition(10, 110);

        window.draw(info_swaps);
        window.draw(info_comparisons);
        window.draw(info_time);

        window.display();
        
        sf::sleep(sf::seconds(30.0/(NUM_BARS * NUM_BARS)));

        if(!sorted) {
            if(j < NUM_BARS - i - 1) {
                if(arr[j] > arr[j+1]) {
                    int temp = arr[j];
                    arr[j] = arr[j+1];
                    arr[j+1] = temp;
                    swapped = true;
                    lastswap = true;
                    swaps++;
                }
                j++;
                comparisons++;
            } else {
                if(!swapped) {
                    sorted = true;
                }
                swapped = false;
                j = 0;
                i++;
            }
        }
    }

    return 0;
}