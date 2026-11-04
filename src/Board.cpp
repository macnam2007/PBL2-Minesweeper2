#include "../headers/Board.hpp"
#include <random>

Board::Board(int cols, int rows, int mineCount) : cols(cols), rows(rows), mineCount(mineCount) {}

Cell* Board::GetCell(const CellPoint& cellPoint)
{
    return &this->cells[cellPoint.y*this->cols + cellPoint.x];
}

// AURA FARMING ĐOẠN NÀY
void Board::Generate(const CellPoint& cellPoint)
{
    // lấy vùng lân cận điểm khởi động để không tại mìn tại ô đó
    std::vector<CellPoint> ban = GetNeighbors(cellPoint);

    // tạo random
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> random_x(0, this->cols-1);
    std::uniform_int_distribution<int> random_y(0, this->rows-1);

    // random_x(gen);

    // Lặp cho đến khi nào tạo đc màn chơi có thể giải    
    while (true)
    {
        int count = 0;
        std::vector<bool> visited(this->cols * this->rows, false);
        // đánh dấu ô đã không được đặt mìn
        for (const CellPoint point : ban)
        { visited[point.y*this->cols+point.x] = true; }
        // đặt mìn
        while(count < this->mineCount)
        {
            int x = random_x(gen);
            int y = random_y(gen);
            if (!visited[y*this->cols+x])
            {
                visited[y*this->cols+x] = true;
                // chuyển trạng thái của các ô thành có mìn
                this->cells[y*this->cols+x].MineToggle();
                // đánh số cho các ô lân cận
                std::vector<CellPoint> neighbors = GetNeighbors(CellPoint(x,y));
                for (const CellPoint point : neighbors)
                {
                    int i = point.y*this->cols+point.x;
                    this->cells[i].SetNumber(this->cells[i].GetNumber() + 1);
                }
                count++;
            }
        }

        // kiểm tra có thể giải không

        // mở ô đầu tiên
        this->cells[cellPoint.y*this->cols+cellPoint.x].OpenCell();
        FloodFill(cellPoint);

        bool checkEnd = false;
        while (!checkEnd)
        {
            checkEnd = true;
        
            for (int i=0 ; i<this->cols*this->rows; i++)
            {
                // nếu ô chưa mở thì bỏ qua
                if (!this->cells[i].Openned()) continue;

                // Bước 1: chuẩn bị nguyên liệu
                std::vector<CellPoint> neighbors = GetNeighbors(CellPoint(i%cols,i/cols));
                int hiddenCount = 0;
                int flaggedCount = 0;
                for (const CellPoint point : neighbors)
                {
                    int i = point.y*this->cols+point.x;
                    if (this->cells[i].GetState() == CellState::Hidden)
                    { hiddenCount++; }
                    if (this->cells[i].GetState() == CellState::Flagged)
                    { flaggedCount++; }
                }

                // Bước 2: let him cook
                // quy luật 1: nếu trong phạm vi hàng xóm của ô có : "số ô bị che" + "số cờ" = number
                // đặt cờ lên các ô bị che 
                if (hiddenCount+flaggedCount == this->cells[i].GetNumber())
                {
                    for (const CellPoint point : neighbors)
                    {
                        int k = point.y*this->cols+point.x;
                        if (this->cells[k].GetState() == CellState::Hidden)
                        { 
                            this->cells[k].ChangeState(CellState::Flagged);
                            checkEnd = false;
                        }
                    }
                }

                // quy luật 2: nếu trong phạm vi hàng xom của ô có : "số cờ" = number
                // mở các ô bị che còn lại
                if (flaggedCount == this->cells[i].GetNumber())
                {
                    for (const CellPoint point : neighbors)
                    {
                        int i = point.y*this->cols+point.x;
                        if (this->cells[i].GetState() == CellState::Hidden)
                        { 
                            this->cells[i].OpenCell();
                            if (this->cells[i].GetNumber() == 0)
                            FloodFill(point);
                            checkEnd = false;
                        }
                    }
                }
            }
        }
        // kiểm tra xem số cờ đặt ra có bằng số mìn có không
        int flaggedCount = 0;
        for (int i =0; i<this->cols*this->rows; i++)
        {
            if (this->cells[i].GetState() == CellState::Flagged)
            { flaggedCount++;}
        }
        // Bàn cờ đã có thể giải -> dừng
        if (flaggedCount == this->mineCount)
        {
            // cần che màn chơi lại chứ là trả về màn đã giải
            for (int i =0; i<this->cols*this->rows; i++)
            { this->cells[i].ChangeState(CellState::Hidden); }
            return;
        }
        // Bàn cờ chưa thể giải bằng quy luật trên -> reset cells và làm lại
        else {
            for (int i =0; i<this->cols*this->rows; i++)
            { this->cells[i].Reset(); }
        }
    }
}

void Board::FloodFill(const CellPoint& cellPoint)
{
    // mở cái ô cellPoint này đã đc mở từ trước và number = 0

    std::vector<bool> visited(this->cols * this->rows, false);
    std::vector<CellPoint> pointNeedOpen;
    std::vector<CellPoint> save;
    pointNeedOpen.push_back(cellPoint);
    while (pointNeedOpen.size() > 0)
    {
        for (const CellPoint point1 : pointNeedOpen)
        {
            std::vector<CellPoint> neighbors = GetNeighbors(point1);
            for (const CellPoint point2 : neighbors)
            {
                int i = point2.y*this->cols+point2.x;

                // hàng xóm đã ghé trước đó thì bỏ qua
                if (visited[i]) continue;

                // đánh dấu đã ghé
                visited[i] = true;

                this->cells[i].OpenCell();

                // hàng xóm mà bằng 0 mà chưa được ghé tới thì tiếp tục lan tỏa yêu thương
                if (this->cells[i].GetNumber() == 0)
                {
                    save.push_back(point2);
                }
            }
        }
        pointNeedOpen = save;
        save.clear();
    }
}


