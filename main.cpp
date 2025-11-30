#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <deque>
#include <windows.h>
#include <chrono>
#include <cassert>

struct Maps
{
    virtual ~Maps() = default;
    virtual std::string init() = 0;
protected:
    std::string map = "";
};
    struct Map1 : Maps
    {
        std::string init() override
        {
            map += "               ";//for count width
            map += "S...#######...#";
            map += "#.#.........#.#";
            map += "#.#######.#####";
            map += "#.#...........#";
            map += "###.########.##";
            map += "#....#........#";
            map += "#.##...#.######";
            map += "#.#####..#....#";
            map += "#.#.#.##.##.#.#";
            map += "#...#.#...#.#..";
            map += "#.###.#.#.#.#.#";
            map += "#...#...#...#.F";

            return map;
        }
    };
    struct Map2 : Maps
    {
        std::string init() override
        {
            map += "               ";//for count width
            map += "..............#";
            map += "#.#.........#.#";
            map += "#.#.....#....##";
            map += "#.#.....#.....#";
            map += "###.....#.#####";
            map += "#....#..#.....#";
            map += "#.####..#....##";
            map += "#.###.........#";
            map += "#.#.#.##.##.#.#";
            map += "#...#.#...#.#..";
            map += "#.###.#.#.#####";
            map += "#..S#...#.....F";

            return map;
        }
    };
    struct Map3 : Maps
    {
        std::string init() override
        {
            map += "               ";//for count width
            map += "F.#############";
            map += "#.#...#...#...#";
            map += "#.#.#.#.#...#.#";
            map += "#...#.#.###.#.#";
            map += "#.###.#.#...###";
            map += "###...#.#.#.#.#";
            map += "#...#...###.#.#";
            map += "#.#.#.#.#.#...#";
            map += "###.###.#.#.#.#";
            map += "#.#.#.#.#.#.#.#";
            map += "#...#...#...#.#";
            map += "#############.S";

            return map;
        }
    };

struct Draw
{
    virtual ~Draw() = default;
    virtual void draw(std::vector<std::vector<std::string>>&) = 0;
};
    struct Mini : Draw
    {
        void draw(std::vector<std::vector<std::string>>& matrix) override
        {
            const int row = matrix.size();
            const int col = matrix[0].size();

            for (int i = 0; i < row; i++)
            {
                for (int j = 0; j < col; j++)
                {
                    if (matrix[i][j] == ".")
                        std::cout << ".. ";
                    else if (matrix[i][j] == "#")
                        std::cout << "## ";
                    else
                        std::cout << (matrix[i][j].length() == 1 ? " " : "") + matrix[i][j] << " ";
                }
                std::cout << "\n";
            }
        }
    };
    struct Maxi : Draw
    {
        void draw(std::vector<std::vector<std::string>>& matrix) override
        {
            const int row = matrix.size();
            const int col = matrix[0].size();
            auto line = [&col]()
                {
                    int k = 0;
                    std::cout << "   +";
                    while (k++ < col) std::cout << " -- +";
                    std::cout << "\n";
                };

            int ff = 0;
            std::cout << "    ";
            while (ff < col) { std::cout << " " << (ff < 10 ? " " : "") << ff++ << "  "; }
            std::cout << "\n";

            int kk = 0;
            std::cout << "   +";
            while (kk++ < col) { std::cout << " -- +"; }
            std::cout << "\n";

            for (int i = 0; i < row; i++)
            {
                //std::cout << "|";
                for (int j = 0; j < col; j++)
                {
                    if (j == 0)
                        std::cout << (i < 10 ? " " : "") << i << " | ";

                    if (matrix[i][j] == ".")
                        std::cout << "   | ";
                    //else if (matrix[i][j] == "#")
                    //    std::cout << "## | ";
                    else
                        std::cout << (matrix[i][j].length() == 1 ? " " : "") + matrix[i][j] << " | ";
                }
                std::cout << "\n";
                line();
            }
        }
    };

struct FindPath
{
public:
    void set_map(std::unique_ptr<Maps> new_map = nullptr)
    {
        map = std::move(new_map);
        init_matrix();
    }
    void set_draw(std::unique_ptr<Draw> new_draw = nullptr)
    {
        draw = std::move(new_draw);
    }
    void algorithm1(size_t sleep = 100)
    {
        auto start = std::chrono::system_clock::now();
        if (draw)
            draw->draw(matrix);
        int startX = point_start.first;
        int startY = point_start.second;
        std::deque<std::pair<int, int>> deque;
        deque.push_back({ startX, startY });
        int count = 0;
        std::pair<int, int> cnt = {};
        while (!deque.empty())
        {
            const auto& current = deque.front();

            startX = current.first;
            startY = current.second;
            count = (!count ? ++count : std::stoi(matrix[startX][startY]) + 1);
            deque.pop_front();

            int arr[5] = { 1,0,-1,0,1 };
            for (int i = 0; i < 4; i++)
            {
                const int x = arr[i] + startX;
                const int y = arr[i + 1] + startY;
                if (x >= 0 && x < matrix.size() && y >= 0 && y < matrix[0].size()
                    && (matrix[x][y] == "." || matrix[x][y] == "F"))
                {
                    if (matrix[x][y] == ".")
                    {
                        deque.push_back({ x,y });
                        matrix[x][y] = std::to_string(count);
                    }
                    else
                    {
                        deque.clear();
                        back_path();
                        for (const auto& it : matrix)
                        {
                            for (const auto& i : it) if (i[0] == '.') ++cnt.first;
                        }
                        break;
                    }
                }
            }
            system("cls");
            draw->draw(matrix);
            Sleep(sleep);
            inf(cnt);
            auto end = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed_seconds = end - start;
            std::cout << "Время работы: " << elapsed_seconds.count() << " сек.\n";
        }
    }
    void algorithm2(size_t sleep = 100)
    {
        auto start = std::chrono::system_clock::now();
        if (draw)
            draw->draw(matrix);
        int startX = point_start.first;
        int startY = point_start.second;
        std::deque<std::pair<int, int>> deque, fast_deque;
        std::deque<std::pair<int, int>>* current_deque = &deque;
        std::vector<std::pair<int, int>> temp_vector;
        temp_vector.reserve(8);
        deque.push_back({ startX, startY });
        int count = 0;
        std::pair<int, int> cnt = {};
        while (!(*current_deque).empty())
        {
            temp_vector.clear();
            const auto& current = (*current_deque).front();

            startX = current.first;
            startY = current.second;
            count = (!count ? ++count : std::stoi(matrix[startX][startY]) + 1);
            (*current_deque).pop_front();

            int arr[5] = { 1,0,-1,0,1 };
            for (int i = 0; i < 4; i++)
            {
                const int x = arr[i] + startX;
                const int y = arr[i + 1] + startY;
                if (x >= 0 && x < matrix.size() && y >= 0 && y < matrix[0].size()
                    && (matrix[x][y] == "." || matrix[x][y] == "F"))
                {
                    if (matrix[x][y] == ".")
                    {
                        temp_vector.push_back({ x,y });
                    }
                    else
                    {
                        deque.clear();
                        fast_deque.clear();
                        back_path();
                        for (const auto& it : matrix)
                        {
                            for (const auto& i : it) if (i[0] == '.') ++cnt.first;
                        }
                        break;
                    }
                }
            }
            if (current_deque == &fast_deque)
            {
                std::cout << "";
            }
            if (temp_vector.size() > 1)
            {
                std::pair<int, int> point_near = point_near_to_finish(temp_vector);
                fast_deque.push_back(point_near);
                if (matrix[point_near.first][point_near.second][0] == '.')
                    matrix[point_near.first][point_near.second] = std::to_string(count);
            }
            for (const auto& it : temp_vector)
            {
                if (current_deque == &fast_deque && fast_deque.empty())
                    fast_deque.push_back(it);
                else
                {
                    deque.push_front(it);
                }
                if (matrix[it.first][it.second][0] == '.')
                    matrix[it.first][it.second] = std::to_string(count);
            }
            current_deque = !fast_deque.empty() ? &fast_deque : &deque;

            system("cls");
            draw->draw(matrix);
            Sleep(sleep);
            inf(cnt);
            auto end = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed_seconds = end - start;
            std::cout << "Время работы: " << elapsed_seconds.count() << " сек.\n";
        }
    }
    void algorithm3(size_t sleep = 100)
    {
        auto start = std::chrono::system_clock::now();
        if (draw)
            draw->draw(matrix);
        int startX = point_start.first;
        int startY = point_start.second;
        std::deque<std::pair<int, int>> deque, fast_deque;
        std::deque<std::pair<int, int>>* current_deque = &deque;
        std::vector<std::pair<int, int>> temp_vector;
        temp_vector.reserve(8);
        deque.push_back({ startX, startY });
        int count = 0;
        std::pair<int, int> cnt = {};
        while (!(*current_deque).empty())
        {
            temp_vector.clear();
            const auto& current = (*current_deque).front();

            startX = current.first;
            startY = current.second;

            count = (!count ? ++count : stoi(matrix[startX][startY]) + 1);
            (*current_deque).pop_front();

            int arr[5] = { 1,0,-1,0,1 };
            for (int i = 0; i < 4; i++)
            {
                const int x = arr[i] + startX;
                const int y = arr[i + 1] + startY;
                if (x >= 0 && x < matrix.size() && y >= 0 && y < matrix[0].size()
                    && (matrix[x][y] == "." || matrix[x][y] == "F"))
                {
                    if (matrix[x][y] == ".")
                    {
                        temp_vector.push_back({ x,y });
                    }
                    else
                    {
                        deque.clear();
                        fast_deque.clear();
                        back_path();
                        for (const auto& it : matrix)
                        {
                            for (const auto& i : it) if (i[0] == '.') ++cnt.first;
                        }
                        break;
                    }
                }
            }
            if (temp_vector.size() > 1)
            {
                std::pair<int, int> point_near = point_near_to_finish(temp_vector);
                if (!deque.empty())
                {
                    const std::pair<int, int> temp_pair = hypotenuse(point_near, deque.front());
                    if (temp_pair == deque.front())
                    {
                        deque.pop_front();
                        fast_deque.push_back(temp_pair);
                        if (matrix[temp_pair.first][temp_pair.second][0] == '.')
                            matrix[temp_pair.first][temp_pair.second] = std::to_string(count);
                    }
                }
                fast_deque.push_back(point_near);
                if (matrix[point_near.first][point_near.second][0] == '.')
                    matrix[point_near.first][point_near.second] = std::to_string(count);
            }
            for (auto it : temp_vector)
            {
                if (current_deque == &fast_deque && fast_deque.empty())
                    fast_deque.push_back(it);
                else
                {
                    if (!deque.empty())
                    {
                        ++cnt.second;
                        const std::pair<int, int> temp_pair = hypotenuse(it, deque.front());
                        if (temp_pair == deque.front())
                        {
                            deque.pop_front();
                            fast_deque.push_back(temp_pair);
                            if (matrix[temp_pair.first][temp_pair.second][0] == '.')
                                matrix[temp_pair.first][temp_pair.second] = std::to_string(count);
                        }
                    }
                    deque.push_front(it);
                }
                if (matrix[it.first][it.second][0] == '.')
                    matrix[it.first][it.second] = std::to_string(count);
            }
            current_deque = !fast_deque.empty() ? &fast_deque : &deque;

            system("cls");
            draw->draw(matrix);
            Sleep(sleep);
        }
        inf(cnt);
        //auto end2 = std::chrono::high_resolution_clock::now();
        //double ms = std::chrono::duration<double, std::milli>(end - start);
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        std::cout << "Время работы: " << elapsed_seconds.count() << " сек.\n";
    }
    std::pair<int, int> hypotenuse(std::pair<int, int>& left, std::pair<int, int>& right)
    {
        const int left_x = abs(point_finish.first - left.first) + 2;
        const int left_y = abs(point_finish.second - left.second) + 2;
        const int right_x = abs(point_finish.first - right.first) + 2;
        const int right_y = abs(point_finish.second - right.second) + 2;

        int res_left = left_x * left_x + left_y * left_y;
        int res_right = right_x * right_x + right_y * right_y;

        return res_left < res_right
                            ? left
                            : right;
    }
    std::pair<int, int> point_near_to_finish(std::vector<std::pair<int, int>>& vector)
    {
        if (vector.empty())
            return { 0,0 };
        else if (vector.size() == 1)
            return vector[0];

        std::pair<int, int> temp_point{ (point_finish.first + 1) * matrix.size(),
                                        (point_finish.second + 1) * matrix[0].size() };

        for (auto& it : vector)
            temp_point = hypotenuse(it, temp_point);

        for (int i = 0; i < vector.size(); i++)
        {
            if (vector[i] == temp_point)
                vector.erase(vector.begin() + i);
        }
        return temp_point;
    }
    void inf(const std::pair<int, int>& cnt) const
    {
        std::cout << "\nВсего возможных ходов: " << kpd
            << "\nСделанно ходов: " << kpd - cnt.first
            << "\nКПД алгоритма: " << (int)((cnt.first / kpd) * 100) << "%"
            << "\nДоп. вызов if( hypotenuse ): " << cnt.second << "\n";
    }
private:
    void back_path()
    {
        int startX = point_finish.first;
        int startY = point_finish.second;
        int num = -1;
        int it = 5;
        while (it > 1)
        {
            int arr[5] = { 1,0,-1,0,1 };
            for (int i = 0; i < 4; i++)
            {
                const int x = arr[i] + startX;
                const int y = arr[i + 1] + startY;
                if (x >= 0 && x < matrix.size() && y >= 0 && y < matrix[0].size()
                    && isdigit(matrix[x][y][0]))
                {
                    const int digit = stoi(matrix[x][y]);
                    if (num == int(digit + 1) || num == int(-1))
                    {
                        it = num = digit;
                        matrix[x][y] = ' ';
                        startX = x;
                        startY = y;
                        break;
                    }
                }
            }
        }
    }
    void init_matrix()
    {
        assert(map && "Select Map");
        std::string map_str = "";
        if (map)
            map_str = map->init();

        size_t k = 0;
        const size_t map_len = map_str.length();

        while (k < map_len && map_str[k] == ' ') ++k;

        const size_t col = k;
        const size_t row = (map_str.length() - k) / k;

        std::vector<std::vector<std::string>> _matrix(row, std::vector<std::string>(col));

        for (size_t i = 0; i < row; i++)
        {
            for (size_t j = 0; j < col; j++)
            {
                if (map_str[k] == 'S')
                {
                    point_start = { i,j };
                    _matrix[i][j] = '.';
                }
                else if (map_str[k] == 'F')
                {
                    point_finish = { i,j };
                    _matrix[i][j] = '.';
                }
                if (map_str[k] == '.')
                {
                    ++kpd;
                    _matrix[i][j] = map_str[k++];
                }
                else
                    _matrix[i][j] = map_str[k++];
                //std::cout << _matrix[i][j] << " ";
            }
            //std::cout << "\n";
        }
        matrix = _matrix;
    }
private:
    std::unique_ptr<Draw> draw;
    std::unique_ptr<Maps> map;
    std::vector<std::vector<std::string>> matrix;
    std::pair<int, int> point_start;
    std::pair<int, int> point_finish;
    float kpd = 0;
};

int main()
{
    setlocale(LC_ALL, "RU");

    FindPath f;
    f.set_map(std::make_unique<Map3>());
    f.set_draw(std::make_unique<Mini>());

    f.algorithm3(300);

    return 0;
}