#include <SFML/Graphics.hpp>

#define SHADER_FILENAME "./shader.glsl"

sf::Vector2f convertMousePos(sf::Vector2i mousePos, sf::RenderWindow &window)
{
    sf::Vector2f convertedPos = window.mapPixelToCoords(mousePos);
    convertedPos.x = convertedPos.x / window.getSize().x;
    convertedPos.y = 1.0f - convertedPos.y / window.getSize().y;
    // std::cout << "Mouse position: " << convertedPos.x << ", " << convertedPos.y << std::endl;

    return convertedPos;
}

sf::Vector2f worldPosition(sf::Vector2f convertedPos, float x_min, float x_max, float y_min, float y_max)
{
    float mandel_x = x_min + (x_max - x_min) * convertedPos.x;
    float mandel_y = y_min + (y_max - y_min) * convertedPos.y;

    sf::Vector2f mandelPos = sf::Vector2f(mandel_x, mandel_y);
    // std::cout << "Mandel position: " << mandelPos.x << ", " << mandelPos.y << std::endl;
    return mandelPos;
}

int main(int argc, char *argv[])
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Mandelbrot");
    window.setFramerateLimit(144);
    float x_min = -2.0f;
    float x_max = 2.0f;
    float y_min = -2.0f;
    float y_max = 2.0f;
    int color = 0;

    sf::Text text;
    sf::Font font;
    font.loadFromFile("./arial.ttf");
    if (!font.loadFromFile("arial.ttf"))
    {
        printf("Failed to load font\n");
    }
    text.setFont(font);

    auto clock = sf::Clock();

    auto shape = sf::RectangleShape(sf::Vector2f(window.getSize()));
    if (!sf::Shader::isAvailable())
    {
        printf("Shaders are not available\n");
        return -1;
    }
    else
    {
        printf("Shaders are available\n");
    }

    sf::Shader shader;
    if (!shader.loadFromFile(SHADER_FILENAME, sf::Shader::Fragment))
    {
        printf("Failed to load shader\n");
        return -1;
    }

    auto mouse_position = sf::Vector2f();
    auto mandel_position = sf::Vector2f();

    while (window.isOpen())
    {
        bool scrolled = false;
        for (auto event = sf::Event(); window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::MouseMoved)
            {
                mouse_position = convertMousePos(sf::Mouse::getPosition(window), window);
                mandel_position = worldPosition(mouse_position, x_min, x_max, y_min, y_max);
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {

                        float delta_x = x_max - x_min;
                        float delta_y = y_max - y_min;
                        x_max = mandel_position.x + delta_x / 2;
                        x_min = mandel_position.x - delta_x / 2;
                        y_max = mandel_position.y + delta_y / 2;
                        y_min = mandel_position.y - delta_y / 2;
                        ;
                    }
                }
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    x_max = 2.0f;
                    x_min = -2.0f;
                    y_max = 2.0f;
                    y_min = -2.0f;
                }

                if (event.key.code == sf::Keyboard::C)
                {
                    color++;
                    if (color > 1)
                    {
                        color = 0;
                    }
                }
            }
            else if (event.type = sf::Event::MouseWheelScrolled)
            {

                scrolled = true;
                if (event.mouseWheelScroll.delta > 0.5)
                {
                    float delta_x = x_max - x_min;
                    float delta_y = y_max - y_min;
                    float center_x = x_min + delta_x / 2;
                    float center_y = y_min + delta_y / 2;
                    x_min = center_x - delta_x / 2 * 1.1f;
                    x_max = center_x + delta_x / 2 * 1.1f;
                    y_min = center_y - delta_y / 2 * 1.1f;
                    y_max = center_y + delta_y / 2 * 1.1f;
                }
                else if (event.mouseWheelScroll.delta < -0.5)
                {
                    float delta_x = x_max - x_min;
                    float delta_y = y_max - y_min;
                    float center_x = x_min + delta_x / 2;
                    float center_y = y_min + delta_y / 2;
                    x_min = center_x - delta_x / 2 * 0.9f;
                    x_max = center_x + delta_x / 2 * 0.9f;
                    y_min = center_y - delta_y / 2 * 0.9f;
                    y_max = center_y + delta_y / 2 * 0.9f;
                }
            }
        }

        shader.setUniform("u_resolution", sf::Glsl::Vec2(window.getSize()));
        shader.setUniform("u_xmax", x_max);
        shader.setUniform("u_xmin", x_min);
        shader.setUniform("u_ymax", y_max);
        shader.setUniform("u_ymin", y_min);
        shader.setUniform("u_color", color);

        text.setString("Zoom: " + std::to_string(2.0f / (x_max - x_min)) + " color: " + std::to_string(color));
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::Red);
        text.setStyle(sf::Text::Regular);

        window.clear();
        window.draw(shape, &shader);
        window.draw(text);
        window.display();
    }
}
