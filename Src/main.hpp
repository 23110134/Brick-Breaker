#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <map>
#include <string>
#include <functional>

// constantes del juego
const int WIDTH = 800;
const int HEIGHT = 600;
const int FPS = 60;

// Ruta de ac
const std::string ASSETS_DIR = "../assets/";

// Cargar imagenes y sonuidos 
struct Assets {
    std::map<std::string, sf::Texture> textures;
    std::map<std::string, sf::SoundBuffer> sounds;
    sf::Font titleFont;
    sf::Font buttonFont;
    
    bool loadTexture(const std::string& name, const std::string& filename) {
        sf::Texture texture;
        if (texture.loadFromFile(filename)) {
            textures[name] = texture;
            return true;
        }
        return false;
    }

    bool loadSound(const std::string& name, const std::string& filename) {
        sf::SoundBuffer buffer;
        if (buffer.loadFromFile(filename)) {
            sounds[name] = buffer;
            return true;
        }
        return false;
    }

    bool loadFont(sf::Font& font, const std::string& filename) {
        return font.loadFromFile(filename);
    }
};

class GameState {
public:
    virtual void events(sf::RenderWindow& window) = 0;
    virtual void update() = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void initialize() = 0;
};

class MainMenuState : public GameState {
private:
    sf::Text title;
    sf::Text playButton;
    Assets& assets;

public:
    MainMenuState(Assets& assets) : assets(assets) {}
    
    void initialize() override {
        title.setFont(assets.titleFont);
        title.setString("Main Menu");
        title.setCharacterSize(160);
        title.setFillColor(sf::Color::White);
        title.setPosition(WIDTH / 2 - title.getGlobalBounds().width / 2, HEIGHT / 4);
        
        playButton.setFont(assets.buttonFont);
        playButton.setString("Press Enter to Start");
        playButton.setCharacterSize(18);
        playButton.setFillColor(sf::Color::White);
        playButton.setPosition(WIDTH / 2 - playButton.getGlobalBounds().width / 2, HEIGHT / 2);
    }
    
    void events(sf::RenderWindow& window) override {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                // Change to another state here, e.g., the actual game
            }
        }
    }

    void update() override {
        // Update logic here
    }

    void draw(sf::RenderWindow& window) override {
        window.clear();
        window.draw(title);
        window.draw(playButton);
        window.display();
    }
};

class Game {
private:
    sf::RenderWindow window;
    sf::Clock clock;
    Assets assets;
    std::unique_ptr<GameState> currentState;

public:
    Game() : window(sf::VideoMode(WIDTH, HEIGHT), "BrickBreaker in SFML") {
        window.setFramerateLimit(FPS);
        loadAssets();
        currentState = std::make_unique<MainMenuState>(assets);
        currentState->initialize();
    }

    void loadAssets() {
        // Load textures
        assets.loadTexture("background", ASSETS_DIR + "images/background.png");
        // Load sounds
        assets.loadSound("brick_sound", ASSETS_DIR + "sound/brick.wav");
        // Load fonts
        assets.loadFont(assets.titleFont, ASSETS_DIR + "fonts/Retronoid.ttf");
        assets.loadFont(assets.buttonFont, ASSETS_DIR + "fonts/Gameplay.ttf");
    }

    void run() {
        while (window.isOpen()) {
            currentState->events(window);
            currentState->update();
            currentState->draw(window);
        }
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}