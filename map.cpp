#include <SFML/Graphics.hpp>
#include <math.h>
#include <vector>

#include <iostream>
#include <fstream>

#define IX(i, j) ((i) + (N) * (j)) 


class Cell : public sf::Sprite
{
public:
    sf::Vector2i pos = sf::Vector2i(0,0);
    int neighbors = 0;
};


int checkNeighbors (int width, int height, Cell cell, Cell *cells[]) {
    int retval = 0;
    int N = width;
    
    if (cell.pos.y != 0) {
        if (cells[IX(cell.pos.x, cell.pos.y - 1)] != nullptr)           // up
            retval = retval | 1;
    }
    /*
    if (cell.pos.y != 0 && cell.pos.x != width) {
        if (cells[IX(cell.pos.x + 1, cell.pos.y - 1)] != nullptr)       // up right
            retval = retval | 2;
    }
    */
    if (cell.pos.x != width) {
        if (cells[IX(cell.pos.x + 1, cell.pos.y)] != nullptr)           // right
            retval = retval | 2;
    }
    /*
    if (cell.pos.x != width && cell.pos.y != height) {
        if (cells[IX(cell.pos.x + 1, cell.pos.y + 1)] != nullptr)       // bottom right
            retval = retval | 8;
    }
    */
    if (cell.pos.y != height) {                                         
        if (cells[IX(cell.pos.x, cell.pos.y + 1)] != nullptr)           // bottom
            retval = retval | 4;
    }
    /*
    if (cell.pos.y != height && cell.pos.x != 0) {                      
        if (cells[IX(cell.pos.x - 1, cell.pos.y + 1)] != nullptr)       // bottom left
            retval = retval | 32;
    }
    */
    if (cell.pos.x != 0) {
        if (cells[IX(cell.pos.x - 1, cell.pos.y)] != nullptr)           // left
            retval = retval | 8;
    }
    /*
    if (cell.pos.x != 0 && cell.pos.y != 0) {
        if (cells[IX(cell.pos.x - 1, cell.pos.y - 1)] != nullptr)       // up left
            retval = retval | 128;
    }
    */
    return retval;
}


void mouse_clicked(sf::Vector2f pos, int width, int height, Cell *recs[], sf::Texture tiles[]) {
    bool found = false;
    int N = width;
    int i;
    sf::Vector2i pos_coord = sf::Vector2i(floor(pos.x / 32.f), floor(pos.y / 32.f));
    if (pos_coord.x < 0 || pos_coord.x >= width || pos_coord.y < 0 || pos_coord.y >= width)
        return;
    for (i = 0; i < height * width; i++) {
        if (recs[i] != nullptr) {
            if (recs[i]->pos == pos_coord) {
                found = true;
                break;
            }
        }
    }
    if (found) {
        recs[i] = nullptr;
    } else {
        recs[IX(pos_coord.x, pos_coord.y)] = new Cell;
        sf::Vector2f recPos(32 * pos_coord.x, 32 * pos_coord.y);
        recs[IX(pos_coord.x, pos_coord.y)]->move(recPos);
        recs[IX(pos_coord.x, pos_coord.y)]->pos = pos_coord;
        recs[IX(pos_coord.x, pos_coord.y)]->setOrigin(2,2);
    }
    
    for (int i = 0; i < width * height; i++) {
        if (recs[i] != nullptr) {
            recs[i]->neighbors = checkNeighbors(width, height, *recs[i], recs);
            recs[i]->setTexture(tiles[recs[i]->neighbors]);
        }
    }
}

void fillTextures(sf::Texture tiles[]) {
    sf::Image tileset;
    tileset.loadFromFile("tileset.png");
    tiles[0 ].loadFromImage(tileset, sf::Rect<int>(3 * 32, 3 * 32, 32, 32));
    tiles[1 ].loadFromImage(tileset, sf::Rect<int>(3 * 32, 2 * 32, 32, 32));
    tiles[2 ].loadFromImage(tileset, sf::Rect<int>(0 * 32, 3 * 32, 32, 32));
    tiles[3 ].loadFromImage(tileset, sf::Rect<int>(0 * 32, 2 * 32, 32, 32));
    tiles[4 ].loadFromImage(tileset, sf::Rect<int>(3 * 32, 0 * 32, 32, 32));
    tiles[5 ].loadFromImage(tileset, sf::Rect<int>(3 * 32, 1 * 32, 32, 32));
    tiles[6 ].loadFromImage(tileset, sf::Rect<int>(0 * 32, 0 * 32, 32, 32));
    tiles[7 ].loadFromImage(tileset, sf::Rect<int>(0 * 32, 1 * 32, 32, 32));
    tiles[8 ].loadFromImage(tileset, sf::Rect<int>(2 * 32, 3 * 32, 32, 32));
    tiles[9 ].loadFromImage(tileset, sf::Rect<int>(2 * 32, 2 * 32, 32, 32));
    tiles[10].loadFromImage(tileset, sf::Rect<int>(1 * 32, 3 * 32, 32, 32));
    tiles[11].loadFromImage(tileset, sf::Rect<int>(1 * 32, 2 * 32, 32, 32));
    tiles[12].loadFromImage(tileset, sf::Rect<int>(2 * 32, 0 * 32, 32, 32));
    tiles[13].loadFromImage(tileset, sf::Rect<int>(2 * 32, 1 * 32, 32, 32));
    tiles[14].loadFromImage(tileset, sf::Rect<int>(1 * 32, 0 * 32, 32, 32));
    tiles[15].loadFromImage(tileset, sf::Rect<int>(1 * 32, 1 * 32, 32, 32));
}

sf::RectangleShape getBound(sf::Vector2i start_pos, sf::Vector2i end_pos, bool vertical) {
    sf::RectangleShape bound;
    bound.setFillColor(sf::Color::Transparent);
    bound.setOutlineThickness(2);
    
    if (vertical) {
        bound.setSize(sf::Vector2f(32*(end_pos.x - start_pos.x) + 26, 32));
        bound.move(sf::Vector2f(32*start_pos.x + 1, 32*start_pos.y - 2));
        bound.setOutlineColor(sf::Color::Blue);
    } else {
        bound.setSize(sf::Vector2f(32, 32*(end_pos.y - start_pos.y) + 26));
        bound.move(sf::Vector2f(32*start_pos.x - 2, 32*start_pos.y + 1));
        bound.setOutlineColor(sf::Color::Cyan);
    }
    
    
    return bound;
}

void setBounds(Cell *recs[], int width, int height, std::vector<sf::RectangleShape> &h_bounds, std::vector<sf::RectangleShape> &v_bounds) {
    int N = width;
    
    bool started = false;
    sf::Vector2i start_pos = sf::Vector2i(0,0);
    sf::Vector2i end_pos = sf::Vector2i(0,0);
    
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            if (!started && recs[IX(i, j)] != nullptr) {
                if (!(recs[IX(i, j)]->neighbors & 1) || !(recs[IX(i, j)]->neighbors & 4)) {
                    /* if we haven't started the bounding box, the point we're looking at
                     * exists, and there is no neighbor above or below the current box, start
                     * the bounding box
                     */
                    started = true;
                    start_pos = recs[IX(i, j)]->pos;
                }
            } else if (started) {
                if (recs[IX(i, j)] != nullptr) {
                    if (recs[IX(i, j)]->neighbors & 1 && recs[IX(i, j)]->neighbors & 4) {
                        /* if we have started the bounding box, the point we're looking
                         * at exists, but there is a box above AND below the current box, end
                         * the bounding box at the previous coordinate
                         */
                        started = false;
                        end_pos = recs[IX(i - 1, j)]->pos;
                        
                        v_bounds.push_back(getBound(start_pos, end_pos, true));
                    }
                } else {
                    /* if we have started the bounding box, but the current point does
                     * not exist, end the bounding box at the previous coordinate
                     */
                    started = false;
                    end_pos = recs[IX(i - 1, j)]->pos;
                    
                    v_bounds.push_back(getBound(start_pos, end_pos, true));
                }
            }
        }
        if (started) {
            /* if the bounding box has not ended, but we have started the next row of
             * points, end the box at the end of the level (to the right)
             */
            started = false;
            end_pos = recs[IX(width - 1, j)]->pos;
            
            v_bounds.push_back(getBound(start_pos, end_pos, true));
        }
    }
    
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (!started && recs[IX(i, j)] != nullptr) {
                if (!(recs[IX(i, j)]->neighbors & 2) || !(recs[IX(i, j)]->neighbors & 8)) {
                    /* if we haven't started the bounding box, the point we're looking at
                     * exists, and there is either no neighbor to the right, or there is
                     * no neighbor to the left of the current box, start the bounding box
                     */
                    
                     
                    started = true;
                    start_pos = recs[IX(i, j)]->pos;
                } 
            } else if (started) {
                if (recs[IX(i, j)] != nullptr) {
                    if (recs[IX(i, j)]->neighbors & 2 && recs[IX(i, j)]->neighbors & 8) {
                        /* if we have started the bounding box, the point we're looking
                         * at exists, but there is a box to the left AND to the right of
                         * the current box, end the bounding box at the previous coordinate
                         */
                        started = false;
                        end_pos = recs[IX(i, j - 1)]->pos;
                        
                        h_bounds.push_back(getBound(start_pos, end_pos, false));
                    }
                } else {
                    /* if we have started the bounding box, but the current point does
                     * not exist, end the bounding box at the previous coordinate
                     */
                     std::cout << "boop" << std::endl;
                    started = false;
                    end_pos = recs[IX(i, j - 1)]->pos;
                    
                    h_bounds.push_back(getBound(start_pos, end_pos, false));
                }
            }
        }
        if (started) {
            /* if the bounding box has not ended, but we have started the next row of
             * points, end the box at the end of the level (to the right)
             */
            started = false;
            end_pos = recs[IX(i, height - 1)]->pos;
            
            h_bounds.push_back(getBound(start_pos, end_pos, false));
        }
    }
}


int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "SFML works!");
    sf::View view(sf::Vector2f(400,400), sf::Vector2f(800,800));
    window.setView(view);
    
    
    int width = 40;
    int height = 40;
    Cell *recs[height*width] = { nullptr };
    
    sf::RectangleShape outline;
    outline.setSize(sf::Vector2f(width * 32, height * 32));
    outline.setFillColor(sf::Color::Transparent);
    outline.setOutlineColor(sf::Color::Red);
    outline.setOutlineThickness(2);
    
    
    sf::Texture tiles[16];
    fillTextures(tiles);
    
    // horizontal and vertical bounds
    std::vector<sf::RectangleShape> h_bounds;
    std::vector<sf::RectangleShape> v_bounds;
    
    sf::Vector2i press_pos = sf::Vector2i(0,0);
    sf::Vector2f view_press_pos = view.getCenter();
    bool mouse_pressed = false;
    bool mouse_dragging = false;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                mouse_pressed = true;
                press_pos = sf::Mouse::getPosition(window);
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                mouse_pressed = false;
                if (mouse_dragging) {
                    mouse_dragging = false;
                } else {
                    sf::Vector2i window_pos = sf::Mouse::getPosition(window);
                    sf::Vector2f pos = window.mapPixelToCoords(window_pos);
                    mouse_clicked (pos, width, height, recs, tiles);
                    h_bounds.clear();
                    v_bounds.clear();
                    setBounds(recs, width, height, h_bounds, v_bounds);
                }
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S) {
                std::ofstream outfile ("level.dat");
                
                
                outfile << width << " " << height << " " << v_bounds.size() << " " << h_bounds.size() <<"\n";
                for (int i = 0; i < width * height; i++) {
                    if (recs[i] == nullptr) {
                        outfile << "0 0 0 0\n";
                    } else {
                        outfile << "1 ";
                        outfile << recs[i]->pos.x << " ";
                        outfile << recs[i]->pos.y << " ";
                        outfile << recs[i]->neighbors << "\n";
                    }
                }
                
                for (int i = 0; i < (int)v_bounds.size(); i++) {
                    sf::Vector2f center = v_bounds[i].getSize() * .5f + v_bounds[i].getPosition();
                    sf::Vector2f size = v_bounds[i].getSize();
                    outfile << center.x << " " << center.y << " " << size.x << " " << size.y << "\n";
                }
                
                for (int i = 0; i < (int)h_bounds.size(); i++) {
                    sf::Vector2f center = h_bounds[i].getSize() * .5f + h_bounds[i].getPosition();
                    sf::Vector2f size = h_bounds[i].getSize();
                    outfile << center.x << " " << center.y << " " << size.x << " " << size.y << "\n";
                }
                
                
                
                outfile.close();
            }
        }
        
        if (mouse_pressed && !mouse_dragging) {
            sf::Vector2i curr_pos = sf::Mouse::getPosition(window);
            sf::Vector2i diff = press_pos - curr_pos;
            if (sqrt(diff.x * diff.x + diff.y * diff.y) > 10) { // if mouse has moved more than 10 pixels
                mouse_dragging = true;
                view_press_pos = view.getCenter();
            }
        }
        
        if (mouse_dragging) {
            sf::Vector2i curr_pos = sf::Mouse::getPosition(window);
            sf::Vector2f diff;
            diff.x = press_pos.x - curr_pos.x;
            diff.y = press_pos.y - curr_pos.y;
            
            view.setCenter(view_press_pos + diff);
            window.setView(view);
        }

        window.clear();
        
        for (int i = 0; i < width * height; i++) {
            if (recs[i] != nullptr)
                window.draw(*recs[i]);
        }
        
        for(int i = 0; i < (int)h_bounds.size(); i++) {
            window.draw(h_bounds[i]);
        }
        for(int i = 0; i < (int)v_bounds.size(); i++) {
            window.draw(v_bounds[i]);
        }
        
        std::cout << h_bounds.size() << ", " << v_bounds.size() << std::endl;
        
        window.draw(outline);
        
        window.display();
    }

    return 0;
}
