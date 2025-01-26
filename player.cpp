#include <iostream>
#include "constants.h"
#include "player.h"
#include "board.h"


void play_game(){
    
    char board_p[ROWS][COLS] = {                  //board to print final moves onto
        {'.','b','b','b','b','b','b','.'},
        {'w','.','.','.','.','.','.','w'},
        {'w','.','.','.','.','.','.','w'},
        {'w','.','.','.','.','.','.','w'},
        {'w','.','.','.','.','.','.','w'},
        {'w','.','.','.','.','.','.','w'},
        {'w','.','.','.','.','.','.','w'},
        {'.','b','b','b','b','b','b','.'}
    };
    char board_t[ROWS][COLS] = {                  //board to print test moves
        {'.','b','b','b','b','b','b','.'},
        {'w','.','.','.','.','.','.','w'},
        {'w','.','.','.','.','.','.','w'},
        {'w','.','.','.','.','.','.','w'},
        {'w','.','.','.','.','.','.','w'},
        {'w','.','.','.','.','.','.','w'},
        {'w','.','.','.','.','.','.','w'},
        {'.','b','b','b','b','b','b','.'}
    };
    

    int piecex{};
    int piecey{};
    int posx{};
    int posy{};

    char currentPlayer = 'b';
    
    Vector2u window_res = {1024,1024};
    int size = 128;
    int offset = 32;
    int mp = 0;
    int dx = 0;
    int dy = 0;
    int piece_x = 0;
    int piece_y = 0;
    bool isMove{false};
    bool validMove{false};
    bool piece_selected{false};
    
    Texture b,B,W;
    b.loadFromFile("./assets/board.png");
    B.loadFromFile("./assets/black.png");
    W.loadFromFile("./assets/white.png");


    Texture PlayT;
    Sprite Play;
    Sprite Board;
    Board.setTexture(b);
    Board.setScale(0.5,0.5);
    Sprite f[24];
    for(int i = 0;i < 24; ++i){
        if(i < 12){
            f[i].setTexture(B);
        }else if(i >= 12&& i < 24){
            f[i].setTexture(W);
        }
    }
    loadPosition(f, size, offset);

    SoundBuffer move_sound;
    SoundBuffer capture_sound;
    SoundBuffer check_sound;
    print_board(board_p);



    bool isWinW = false;
    bool isWinB = false;
    bool isDraw = false;
    bool valid_piece = false;

    move_sound.loadFromFile("./assets/sound/move-self.mp3");
    Sound move_piece(move_sound);

    capture_sound.loadFromFile("./assets/sound/capture.mp3");
    Sound capture_piece(capture_sound);

    //add an option to change a piece, probably just a while loop
    sf::RenderWindow window(sf::VideoMode({window_res.x, window_res.y}), "Lines of Action",Style::Titlebar|sf::Style::Close);

    while(window.isOpen()){
        Vector2i pos = Mouse::getPosition(window);
        Event e;
        while(window.pollEvent(e)){
            if(e.type == Event::Closed)
                window.close();

            if(e.type == Event::MouseButtonPressed)
                if(e.key.code == Mouse::Left){
                    for(int i = 0; i < 24;++i){
                        if(f[i].getGlobalBounds().contains(pos.x,pos.y)){
                            int temp_x = int(pos.x/size);
                            int temp_y = int(pos.y/ size);
                            valid_piece = board_p[temp_y][temp_x] != '.' && isPiece(board_p, temp_y, temp_x, currentPlayer);
                            if(valid_piece){    
                                mp = i;
                                isMove = true;
                                dx = pos.x - f[i].getPosition().x;
                                dy = pos.y - f[i].getPosition().y;
                                piece_x = temp_x;
                                piece_y = temp_y;
                            }
                        }
                    }
                }
                
                if(e.type == Event::MouseButtonReleased){
                    if(e.key.code == Mouse::Left){
                        Vector2f p = f[mp].getPosition() + Vector2f(size/2,size/2); 
                        std::pair <int,int> dest(int(p.x/size),int(p.y/size)); 
                        get_possible_moves(board_p, board_t, piece_y, piece_x, currentPlayer);
                        print_board(board_t);
                        validMove = valid_move(board_t,dest.second, dest.first, piece_y, piece_x,currentPlayer);

                        isMove = false;
                                  
                        Vector2f newPos = Vector2f(size*int(p.x/size)+offset,size*int(p.y/size)+offset);
                        
                        if(newPos.x < float(window_res.x) && newPos.y < float(window_res.y)){
                            if(valid_piece && validMove){
                                    if(board_p[dest.second][dest.first] == '.')
                                        move_piece.play();
                                    else
                                        capture_piece.play();
                                add_move(board_p, dest.second, dest.first, piece_y, piece_x, currentPlayer);
                                guiCapture(newPos,f,currentPlayer);
                                f[mp].setPosition(newPos);
                                resetBoard(board_t, board_p);
                                switchTurn(currentPlayer);

                                isDraw = check_draw(board_p);
                            
                                if(isDraw == true){
                                    guiDraw();
                                    window.close();
                                } 
                                isWinW = are_all_pieces_adjacent(board_p,'w');
                                isWinB =  are_all_pieces_adjacent(board_p,'b');
                                if(isDraw == false){
                                    if(isWinW == true || isWinB == true){
                                        std::string Player{};
                                        std::string x_wins = " wins";
                                        if(isWinB){
                                            guiWinner('b');
                                
                                        }else if(isWinW){
                                            guiWinner('w');
                                            
                                        }
                                        std::string message = Player + x_wins;
                                        window.close();
                                    }
                                }
                                                
                                
                           
                            }else if(!validMove){
                                if(board_p[piece_y][piece_x] != '.' && valid_piece){
                                    f[mp].setPosition(Vector2f(piece_x*size +offset, piece_y*size+offset) );
                                    resetBoard(board_t, board_p);
                                }
                            }
                        }
                    }
                }
                    
                
                
        }

        if(isMove) f[mp].setPosition(Vector2f(pos.x-dx, pos.y - dy) );

        window.clear(Color::White);
        window.setVerticalSyncEnabled(true);
        window.draw(Board);
        for(int i{}; i < 24; ++i) window.draw(f[i]);
        window.display();
    }

    return;
}

void menu(){
    sf::RenderWindow menu(sf::VideoMode({1024,1024}), "Lines of Action", Style::Titlebar|sf::Style::Close);
    Texture PlayT;
    PlayT.loadFromFile("./assets/buttons/buttons.png");
    Texture bg;
    bg.loadFromFile("./assets/bg.jpeg");

    Sprite Play(PlayT);
    Sprite background(bg);
    background.setPosition(-280,0);
    
    Play.setPosition(350,447);

    float px{};
    float py{};
    while(menu.isOpen()){
        Vector2i pos = Mouse::getPosition(menu);
        Event e;
        while(menu.pollEvent(e)){
            if(e.type == Event::Closed)
                menu.close();
            if(Play.getGlobalBounds().contains(pos.x,pos.y)){
                Play.setTextureRect(IntRect(330,0,318,130));
            }else{
                Play.setTextureRect(IntRect(0,0,325,130));
            }
            if(e.type == Event::MouseButtonPressed){
                if(e.key.code == Mouse::Left){
                    if(Play.getGlobalBounds().contains(pos.x,pos.y)){
                        menu.close();
                        play_game();
                    }
                }
            }                         
        }
        //draw
        menu.clear(sf::Color::White);
        menu.setVerticalSyncEnabled(true);
        menu.draw(background);
        menu.draw(Play);
        menu.display();
    }
    

}

void guiWinner(char Player){
    std::string colour{};
    if(Player == 'w'){
        colour = "White ";
    }else if(Player == 'b'){
        colour = "Black ";
    }

    std::string msg = colour + "Wins";
    sf::Text win;
    sf::Font sans_font;

    sans_font.loadFromFile("./assets/font/sans.ttf");
    win.setFont(sans_font);
    win.setString(msg);
    win.setCharacterSize(70);
    win.setFillColor(sf::Color::Black);
    win.setPosition(40,50);

    RenderWindow winner(sf::VideoMode({400,200}),"Result", Style::Titlebar|sf::Style::Close);

    while(winner.isOpen()){
        Event eS;
        while(winner.pollEvent(eS)){
            if(eS.type == Event::Closed)
                winner.close();                        
        }
        
        winner.clear(sf::Color::White);
        winner.draw(win);
        winner.display();

    }

}

void guiDraw(){
    
    std::string msg = "Draw";
    sf::Text draw;
    sf::Font sans_font;

    sans_font.loadFromFile("./assets/font/sans.ttf");
    draw.setFont(sans_font);
    draw.setString(msg);
    draw.setCharacterSize(70);
    draw.setFillColor(sf::Color::Black);
    draw.setPosition(125,50);

    RenderWindow draw_window(sf::VideoMode({400,200}),"Result", Style::Titlebar|sf::Style::Close);

    while(draw_window.isOpen()){
        Event eS;
        while(draw_window.pollEvent(eS)){
            if(eS.type == Event::Closed)
                draw_window.close();                        
        }
        
        draw_window.clear(sf::Color::White);
        draw_window.draw(draw);
        draw_window.display();

    }

}

void guiCapture(Vector2f newPos,Sprite f[24], char currentPlayer){
    for(int i{}; i < 24; ++i){
       if(f[i].getPosition().x == newPos.x && f[i].getPosition().y == newPos.y){
            f[i].setPosition(Vector2f(1024+378, 0));
       } 
    }
}

void resetBoard(char board_t[8][8], char board_p[8][8]){
    for(int i{}; i < 8; ++i){
        for(int j{}; j < 8; ++j){
            board_t[i][j] = board_p[i][j];
        }
    }
}

void switchTurn(char& Player){
    if(Player == 'b'){
        Player = 'w';
    }else if(Player == 'w'){
        Player = 'b';
    }
}

void loadPosition(Sprite f[24], int size, int offset){
    int n = 24;

    for(int i = 0; i < n/4; ++i){
        f[i].setPosition((i+1)*size+offset,offset);
    }

    for(int i = 0; i < n/4; ++i){
        f[i+6].setPosition((i+1)*size+offset, 7*size + offset);
    }

    for(int i = 0; i < n/4; ++i){
        f[i+12].setPosition(offset, (i+1)*size +offset);
    }

    for(int i = 0; i < n/4; ++i){
        f[i+18].setPosition(7*size + offset, (i+1)*size +offset);
    }


}

void get_possible_moves(char board_f[ROWS][COLS],char board_t[ROWS][COLS], int pieceX, int pieceY, char player){
    //this function gets all the possible moves on the vertical, horizontal and diagonals.
    int vertC{};
    int horC{};
    int odiagC{};
    int diagC{};
    int newXb{};
    int newYb{};

    if(player == 'b'){
        countbw(board_t, pieceX, pieceY, player , vertC, horC, odiagC, diagC);
        
        if(vertC >= 1){ //final
            newXb = pieceX + vertC;
            if(newXb >= 0 && newXb < 8){
                if(board_t[newXb][pieceY] != 'b') { // so that it overwrites only w's and .'s
                    board_t[newXb][pieceY] = '*'; 
                }
                for(int i{pieceX}; i < newXb; ++i){
                    if(board_t[i][pieceY] == 'w'){
                        if(newXb >= 0 && newXb < 8){
                            for(int i{pieceX}; i <= newXb; ++i){
                                board_t[i][pieceY] = board_f[i][pieceY];
                            }
                            break;
                        }
                    }   
                } 
            }
            newXb = 0;     

            newXb = pieceX - vertC;
            if(newXb >= 0 && newXb < 8){
                if(board_t[newXb][pieceY] != 'b'){
                    board_t[newXb][pieceY] = '*'; 
                }
                
                for(int i{newXb}; i < pieceX; ++i){
                    if(board_t[i][pieceY] == 'w'){
                        if(newXb >= 0 && newXb < 8){
                            for(int i{newXb}; i <= pieceX; ++i){
                                board_t[i][pieceY] = board_f[i][pieceY];
                            }
                            break;
                        }
                    }   
                }
            }
            newXb = 0;   
        }
        

        if(horC >= 1){
            newYb = pieceY + horC;
            if(newYb >= 0 && newYb < 8){
                if(board_t[pieceX][newYb] != 'b'){
                    board_t[pieceX][newYb] = '*';
                }
                
                for(int j{pieceY}; j < newYb; ++j){
                    if(board_t[pieceX][j] == 'w'){
                        if(newYb >= 0 && newYb < 8){
                             for(int j{pieceY}; j <= newYb; ++j){
                                board_t[pieceX][j] = board_f[pieceX][j];
                             }
                            break;
                        }
                    }   
                }   
            }
            newYb = 0;

            newYb = pieceY - horC;
            if(newYb >= 0 && newYb < 8){
                if(board_t[pieceX][newYb] != 'b'){
                        board_t[pieceX][newYb] = '*';
                }
    
                for(int j{newYb}; j < pieceY; ++j){
                    if(board_t[pieceX][j] == 'w'){
                        if(newYb >= 0 && newYb < 8){
                            for(int j{newYb}; j <= pieceY; ++j){
                                board_t[pieceX][j] = board_f[pieceX][j];
                            }
                            break;
                        }
                    }   
                }
            }
            newYb = 0;
        }

        
        if(odiagC >= 1){
           newYb = pieceY + odiagC;
           newXb = pieceX + odiagC;
           if((newYb >= 0 && newYb < 8) && (newXb >= 0 && newXb < 8)){
                if(board_t[newXb][newYb] != 'b'){
                    board_t[newXb][newYb] = '*';
                }
           
            int odc = pieceY - pieceX;
            int j{};
            for(int i{pieceX}; i < newXb; ++i){
                    j = i + odc;
                    if(board_t[i][j] == 'w'){ 
                        for(int i{pieceX}; i <= newXb; ++i){
                            j = i + odc;
                            board_t[i][j] = board_f[i][j];
                        }    
                        break;
                    }               
                }
            }
            newYb = 0;
            newXb = 0;

            newYb = pieceY - odiagC;
            newXb = pieceX - odiagC;
            if((newYb >= 0 && newYb < 8) && (newXb >= 0 && newXb < 8)){ 
                if(board_t[newXb][newYb] != 'b'){
                    board_t[newXb][newYb] = '*';
                }
            
                int odc = pieceY - pieceX;
                int j{};
                for(int i{newXb+1}; i < pieceX ;++i){
                    j = i + odc;             
                    if(board_t[i][j] == 'w'){
                        for(int i{newXb}; i <= pieceX ;++i){
                            j = i + odc;
                            board_t[i][j] = board_f[i][j];        
                        } 
                        break;
                    }
                    
                }
            }
            newYb = 0;
            newXb = 0;

        }

        if(diagC >= 1){
            newXb = pieceX + diagC;
            newYb = pieceY - diagC;
            if((newYb >= 0 && newYb < 8) && (newXb >= 0 && newXb < 8)){
                if(board_t[newXb][newYb] != 'b'){
                    board_t[newXb][newYb] = '*';
                }
            
                int dc = pieceY + pieceX;
                int j{};
                for(int i{pieceX}; i < newXb ;++i){
                    j = -i + dc;
                    if(board_t[i][j] == 'w'){
                        for(int i{pieceX}; i <= newXb ;++i){
                            j = -i + dc;
                            board_t[i][j] = board_f[i][j];        
                        }      
                        break;
                    }
                }
            }
            newYb = 0;
            newXb = 0;

            newXb = pieceX - diagC;
            newYb = pieceY + diagC;
            if((newYb >= 0 && newYb < 8) && (newXb >= 0 && newXb < 8)){
                if(board_t[newXb][newYb] != 'b'){
                    board_t[newXb][newYb] = '*';
                }
            
                int dc = pieceX + pieceY;
                int j{};
                for(int i{newXb}; i < pieceX ;++i){
                    j = -i + dc; 
                    if(board_t[i][j] == 'w'){
                        for(int i{newXb}; i <= pieceX ;++i){
                            j = -i + dc;
                            board_t[i][j] = board_f[i][j];        
                        }  
                        break;
                    }
                }
            } 
            newXb = 0;
            newYb = 0;   
        }

        print_board(board_t);
    }

    if(player == 'w'){
        countbw(board_t, pieceX, pieceY, player , vertC, horC, odiagC, diagC);
        
        if(vertC >= 1){ //final
            newXb = pieceX + vertC;
            if(newXb >= 0 && newXb < 8){
                if(board_t[newXb][pieceY] != 'w') { // so that it overwrites only b's and .'s
                    board_t[newXb][pieceY] = '*'; 
                }
                // can probably add a conditional so that it doesnt get evaluated for no reason :v
                for(int i{pieceX}; i < newXb; ++i){
                    if(board_t[i][pieceY] == 'b'){
                        if(newXb >= 0 && newXb < 8){
                            for(int i{pieceX}; i <= newXb; ++i){
                                board_t[i][pieceY] = board_f[i][pieceY];
                            }
                            break;
                        }
                    }   
                } 
            }     
            newXb = 0;

            newXb = pieceX - vertC;
            if(newXb >= 0 && newXb < 8){
                if(board_t[newXb][pieceY] != 'w'){
                    board_t[newXb][pieceY] = '*'; 
                }
                
                for(int i{newXb}; i < pieceX; ++i){
                    if(board_t[i][pieceY] == 'b'){
                        if(newXb >= 0 && newXb < 8){
                            for(int i{newXb}; i <= pieceX; ++i){
                                board_t[i][pieceY] = board_f[i][pieceY];
                            }
                            break;
                        }
                    }   
                }
            } 
            newXb = 0;  
        }

        if(horC >= 1){
            newYb = pieceY + horC;
            if(newYb >= 0 && newYb < 8){
                if(board_t[pieceX][newYb] != 'w'){
                    board_t[pieceX][newYb] = '*';
                }
                
                for(int j{pieceY}; j < newYb; ++j){
                    if(board_t[pieceX][j] == 'b'){
                        if(newYb >= 0 && newYb < 8){
                             for(int j{pieceY}; j <= newYb; ++j){
                                board_t[pieceX][j] = board_f[pieceX][j];
                             }
                            break;
                        }
                    }   
                }   
            }
            newYb = 0;

            newYb = pieceY - horC;
            if(newYb >= 0 && newYb < 8){
                if(board_t[pieceX][newYb] != 'w'){
                        board_t[pieceX][newYb] = '*';
                }
    
                for(int j{newYb}; j < pieceY; ++j){
                    if(board_t[pieceX][j] == 'b'){
                        if(newYb >= 0 && newYb < 8){
                            for(int j{newYb}; j <= pieceY; ++j){
                                board_t[pieceX][j] = board_f[pieceX][j];
                            }
                            break;
                        }
                    }   
                }
            }
            newYb = 0;
        }
        
        if(odiagC >= 1){
           newYb = pieceY + odiagC;
           newXb = pieceX + odiagC;
           if((newYb >= 0 && newYb < 8) && (newXb >= 0 && newXb < 8)){
                if(board_t[newXb][newYb] != 'w'){
                    board_t[newXb][newYb] = '*';
                }
           
            int odc = pieceY - pieceX;
            int j{};
            for(int i{pieceX}; i < newXb; ++i){
                    j = i + odc;
                    if(board_t[i][j] == 'b'){ 
                        for(int i{pieceX}; i <= newXb; ++i){
                            j = i + odc;
                            board_t[i][j] = board_f[i][j];
                        }    
                        break;
                    }               
                }
            }       
            newYb = 0;
            newXb = 0;

            newYb = pieceY - odiagC;
            newXb = pieceX - odiagC;
            if((newYb >= 0 && newYb < 8) && (newXb >= 0 && newXb < 8)){ 
                if(board_t[newXb][newYb] != 'w'){
                    board_t[newXb][newYb] = '*';
                }
            
                int odc = pieceY - pieceX;
                int j{};
                for(int i{newXb+1}; i < pieceX ;++i){
                    j = i + odc;                
                    if(board_t[i][j] == 'b'){
                        for(int i{newXb}; i <= pieceX ;++i){
                            j = i + odc;
                            board_t[i][j] = board_f[i][j];        
                        } 
                        break;
                    }
                    
                }
            }
            newYb = 0;
            newXb = 0;

        }

        if(diagC >= 1){
            newXb = pieceX + diagC;
            newYb = pieceY - diagC;
            if((newYb >= 0 && newYb < 8) && (newXb >= 0 && newXb < 8)){
                if(board_t[newXb][newYb] != 'w'){
                    board_t[newXb][newYb] = '*';
                }
            
                int dc = pieceY + pieceX;
                int j{};
                for(int i{pieceX}; i < newXb ;++i){
                    j = -i + dc;
                    if(board_t[i][j] == 'b'){
                        for(int i{pieceX}; i <= newXb ;++i){
                            j = -i + dc;
                            board_t[i][j] = board_f[i][j];        
                        }      
                        break;
                    }
                }
            }
            newYb = 0;
            newXb = 0;

            newXb = pieceX - diagC;
            newYb = pieceY + diagC;

            if((newYb >= 0 && newYb < 8) && (newXb >= 0 && newXb < 8)){
                if(board_t[newXb][newYb] != 'w'){
                    board_t[newXb][newYb] = '*';
                }
            
                int dc = pieceX + pieceY;
                int j{};
                for(int i{newXb}; i < pieceX ;++i){
                    j = -i + dc;
                    if(board_t[i][j] == 'b'){
                        for(int i{newXb}; i <= pieceX ;++i){
                            j = -i + dc;
                            board_t[i][j] = board_f[i][j];        
                        }  
                        break;
                    }
                }
            }    
        }
        newXb = 0;
        newYb = 0;
        print_board(board_t);
    }

}

//code should be fine after this. emphasis on should
void countbw(char board_t[ROWS][COLS], int pieceX, int pieceY, char player, int& vertMove, int& horMove, int& odMove, int& diagMove) {
    int countV{};//upwards vertical
    int countH{};// right horizontal
    int countod{};//d for diagonal
    int countd{};  
    for(size_t i{}; i < ROWS; ++i){
        if(board_t[i][pieceY] == 'b' || board_t[i][pieceY] == 'w'){
            ++countV;
        }
    }
    vertMove = countV;
    
                      
    for(size_t j{}; j < COLS; ++j){ //horizontal count
        if(board_t[pieceX][j] == 'b'|| board_t[pieceX][j] == 'w'){
            ++countH;
        }
    }
    horMove = countH;

    
    int odc = pieceY - pieceX;// using y = mx formula to calcuate
    size_t y{};

    for(size_t i{}; i < ROWS; ++i){
        y = i + odc;
        if(is_valid(i, y)){
            if(board_t[i][y] == 'b' || board_t[i][y] == 'w'){
                   ++countod;
            }
        }    
    }
    odMove = countod; //off diagonal 


    int dc = pieceY + pieceX;   //diag
    y = 0;
    for(size_t i{}; i < ROWS; ++i){
        y = -i + dc;
        if(is_valid(i, y)){
            if(board_t[i][y] == 'b' || board_t[i][y] == 'w'){
                ++countd;
            }
        } 
    }
    diagMove = countd;

} 

//if you dont give appropriate inputs the program breaks. dont know how to force a certain input.
void get_player_move(char board_f[ROWS][COLS],char board_t[ROWS][COLS], int row, int col, int pieceX, int pieceY, char player){
    char cont{'y'};

    if(player){
        while(cont == 'y'){
            std::cout << "Choose piece. Player: " << player << std::endl;
            std::cin >> pieceX;
            std::cin >> pieceY;
            if(board_t[pieceX][pieceY] == player){
                std::cout << "Valid piece selected." << std::endl;
                get_possible_moves(board_f, board_t, pieceX, pieceY, player);
            }else{
                for(size_t i{}; i < ROWS; ++i){
                    for(size_t j{}; j < COLS; ++j){
                        board_t[i][j] = board_f[i][j];
                    }
                }
                std::cout << "Invalid piece selected, try again." << std::endl;
                bool pieceCheck = false;
                while(pieceCheck == false){
                    pieceX = 0;
                    pieceY = 0;
                    std::cin >> pieceX;
                    std::cin >> pieceY;
                    pieceCheck = isPiece(board_t,pieceX,pieceY, player);
                }
                get_possible_moves(board_f, board_t, pieceX, pieceY, player);
            }
            std::cout << "Choose another piece? y/n" << std::endl;
            std::cin >> cont;
            if(cont == 'y'){
                pieceX = 0;
                pieceY = 0;
                for(size_t i{}; i < ROWS; ++i){
                    for(size_t j{}; j < COLS; ++j){
                        board_t[i][j] = board_f[i][j];
                    }
                }
                print_board(board_t);
            }
        }
        std::cout << "Input a position to move the piece to." << std::endl;
        print_possible_moves(board_t);
        std::cin >> row;
        std::cin >> col;
        bool valid_mv = valid_move(board_t, row, col, pieceX, pieceY, player);
        if(valid_mv){
            add_move(board_f, row, col, pieceX, pieceY, player);
            //clear();
            for(size_t i{}; i < ROWS; ++i){
                for(size_t j{}; j < COLS; ++j){
                    board_t[i][j] = board_f[i][j];
                }
            }
            print_board(board_f);
        }else{
            while(valid_mv == false){
                row = 0;
                col = 0;
                std::cout << "Input valid a move: " << std::endl;
                std::cin >> row;
                std::cin >> col;
                valid_mv = valid_move(board_t, row, col, pieceX, pieceY, player);
                
            }
            add_move(board_f, row, col, pieceX, pieceY, player);
            //clear();
            for(size_t i{}; i < ROWS; ++i){
                for(size_t j{}; j < COLS; ++j){
                    board_t[i][j] = board_f[i][j];
                }
            }
            print_board(board_f);
        
        }
    }
}

bool valid_move(char board_t[ROWS][COLS], int row, int col, int pieceX, int pieceY, char player){
    bool vali = false;
    if(player){
        if(board_t[row][col] == '*'){
            vali = true;
        }else{
            vali = false;
        }
    }
    return vali;
    
}

void add_move(char board_f[ROWS][COLS], int row, int col, int pieceX, int pieceY, char player){
    if(player){
        board_f[pieceX][pieceY] = '.';
        board_f[row][col] = player;
    }
}

bool isPiece(char board_t[ROWS][COLS] ,int pieceX,int pieceY,char player){
    bool piece = false;
    if(player){
        if(board_t[pieceX][pieceY] == player){
            std::cout << "Valid piece selected." << std::endl;
            piece = true;
        }else{
            std::cout << "Invalid piece selected, try again." << std::endl;
            piece = false;
        }
    }
    return piece;
}

void print_possible_moves(char board_t[ROWS][COLS]){
    for(size_t i{}; i < ROWS; ++i){
        for(size_t j{}; j < COLS; ++j){
            if(board_t[i][j] == '*'){
                std::cout << "* @ " << i << " " << j << '\n';
            }
        }
    }
}
