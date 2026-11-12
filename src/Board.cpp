#include "../headers/Board.hpp"
#include <iostream>
#include <random>

Board::Board(int cols, int rows, int mineCount) : cols(cols), rows(rows), mineCount(mineCount)
{ this->cells.resize(cols * rows); }

Cell* Board::GetCell(const CellPoint& cellPoint)
{
    if (cellPoint.x < 0 || cellPoint.x >= this->cols || cellPoint.y < 0 || cellPoint.y >= this->rows)
        return nullptr;

    return &this->cells[cellPoint.y*this->cols + cellPoint.x];
}

// AURA FARMING ĐOẠN NÀY
void Board::Generate(const CellPoint& cellPoint)
{
    if (this->generated) return;

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
        for (const CellPoint& point : ban)
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
                for (const CellPoint& point : neighbors)
                {
                    int i = point.y*this->cols+point.x;
                    this->cells[i].SetNumber(this->cells[i].GetNumber() + 1);
                }
                count++;
            }
        }

        // mở ô đầu tiên
        this->cells[cellPoint.y*this->cols+cellPoint.x].OpenCell();
        FloodFill(cellPoint);

        // ngay click đầu tiên mà win thì làm lại bàn mới
        if (IsWon())
        {
            ResetBoard();
            continue;
        }

        bool checkEnd = false;
        while (!checkEnd)
        {
            checkEnd = true;
        
            for (int i=0 ; i<this->cols*this->rows; i++)
            {
                // chỉ mở các ô chưa được mở và có trạng thái CellState::Hidden 
                if (!this->cells[i].Openned() || this->cells[i].GetNumber() == 0) continue;

                // Bước 1: chuẩn bị nguyên liệu
                std::vector<CellPoint> neighbors = GetNeighbors(CellPoint(i%cols,i/cols));
                int hiddenCount = 0;
                int flaggedCount = 0;
                for (const CellPoint& point : neighbors)
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
                    for (const CellPoint& point : neighbors)
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
                    for (const CellPoint& point : neighbors)
                    {
                        int i = point.y*this->cols+point.x;
                        if (this->cells[i].GetState() == CellState::Hidden)
                        { 
                            this->cells[i].OpenCell();
                            this->countClickTB++;
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
            this->generated = true;
            return;
        }
        // Bàn cờ chưa thể giải bằng quy luật trên -> reset cells và làm lại
        else ResetBoard();
    }
}

void Board::FloodFill(const CellPoint& cellPoint)
{
    // mở cái ô cellPoint này đã đc mở từ trước và number = 0

    std::vector<bool> visited(this->cols * this->rows, false);
    visited[cellPoint.y*this->cols + cellPoint.x] = true;
    std::vector<CellPoint> pointNeedOpen;
    std::vector<CellPoint> save;
    pointNeedOpen.push_back(cellPoint);
    while (pointNeedOpen.size() > 0)
    {
        for (const CellPoint& point1 : pointNeedOpen)
        {
            std::vector<CellPoint> neighbors = GetNeighbors(point1);
            for (const CellPoint& point2 : neighbors)
            {
                int i = point2.y*this->cols+point2.x;

                // hàng xóm đã ghé trước đó thì bỏ qua
                if (visited[i] || !this->cells[i].CanOpenCell()) continue;
                
                // hỏi tổng đài ô này có được phép tương tác không
                if (this->validator && !this->validator(point2)) continue;

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

int Board::GetCols() const
{ return this->cols; }

int Board::GetRows() const
{ return this->rows; }


void Board::ResetBoard()
{
    for (int i =0; i<this->cols*this->rows; i++)
    { this->cells[i].Reset(); }
    SetEnded(false);
    SetLosed(false);
    this->generated = false;
    this->cellPointDead.clear();
    this->countClickTB = 0;
    this->countClick = 0;
}

bool Board::IsGenerated() const
{ return this->generated; }

OpenResult Board::OpenAt(const CellPoint& cellPoint)
{
    Cell* cell = &this->cells[cellPoint.y*this->cols+cellPoint.x];
    // ô mở là mìn
    if (cell->CheckIsMine())
    {
        this->ended = true;
        this->losed = true;

        AddCellPointDead(cellPoint);
        return OpenResult::Lose;
    }

    // mở lần đầu
    Generate(cellPoint);
    if (cell->GetNumber() == 0)
    {
        cell->OpenCell();

        FloodFill(cellPoint);
        if (CheckWin())
        {
            this->ended = true;
            this->losed = false;
            return OpenResult::Win;
        }

        return OpenResult::Safe;
    }
    else {
        if (cell->Openned())
        { return OpenChord(cellPoint); }

        cell->OpenCell();
        if (CheckWin())
        {
            this->ended = true;
            this->losed = false;
            return OpenResult::Win;
        }
    }
    
    return OpenResult::Safe;
}

OpenResult Board::OpenChord(const CellPoint& cellPoint)
{
    Cell* cellCenter = &this->cells[cellPoint.y*this->cols + cellPoint.x];

    // đếm số ô đánh dấu trong 8 ô xung quanh
    std::vector<CellPoint> neighbors = GetNeighbors(cellPoint);
    int count = 0;
    for (const CellPoint& point : neighbors)
    {
        Cell* cell = &this->cells[point.y*this->cols+point.x];
        if (cell->GetState() == CellState::Flagged) { count ++ ;}
        // vì validator xét Questioned vẫn true nên phải kiểm tra lần nx
        if (cell->GetState() == CellState::Questioned) { return OpenResult::Blocked; }
    }

    if (cellCenter->GetNumber() != count)
    { return OpenResult::Blocked; }

    // khi number của ô trung tâm bằng số cờ xung quanh thì mở các ô còn lại
    else
    {
        // Kiểm tra có đẳm mìn không
        for (const CellPoint& point : neighbors)
        {
            // nếu mọc cây thì k đc ktra ô này
            if (this->validator && !this->validator(point)) {continue; }

            Cell* cell = &this->cells[point.y*this->cols+point.x];
            
            if (cell->GetState() == CellState::Hidden && cell->CheckIsMine())
            {
                SetEnded(true);
                SetLosed(true);
                AddCellPointDead(point);
            }
        }
        //TH : đẳm mìn trả về Lose
        if (IsEnded() && IsLosed())
        { return OpenResult::Lose; }
        //TH : không đẳm mìn thì mở các ô
        for (const CellPoint& point : neighbors)
        {
            // nếu mọc cây thì k đc mở ô này
            if (this->validator && !this->validator(point)) {continue; }

            Cell* cell = &this->cells[point.y*this->cols+point.x];
            if (cell->GetState() == CellState::Hidden)
            {
                cell->OpenCell();
                // ô vừa mở có number = 0
                if (cell->GetNumber() == 0)
                { FloodFill(point); }
            }
        }
    }
    if (CheckWin())
    {
        this->ended = true;
        this->losed = false;
        return OpenResult::Win;
    }
    return OpenResult::Safe;
}

void Board::ToggleMark(const CellPoint& cellPoint)
{
    if (this->generated == false) { return; }
    Cell* cell = &this->cells[cellPoint.y*this->cols + cellPoint.x];

    switch (cell->GetState())
    {
        case CellState::Openned:
            break;
        case CellState::Hidden:
            cell->ChangeState(CellState::Flagged);
            break;
        case CellState::Flagged:
            cell->ChangeState(CellState::Questioned);
            break;
        case CellState::Questioned:
            cell->ChangeState(CellState::Hidden);
            break;
    }
}

bool Board::CheckWin()
{
    if (IsWon())
    {
        SetEnded(true);
        SetLosed(false);
        return true;
    }
    return false;
}

bool Board::Load(std::istream& input, float& time) 
{
    /* Mô tả file data */
    //1) dòng đầu tiên "rows cols minesCount timeSave generated ended losed countClick countClickTB"
    //2) rows dòng tiếp theo : chứa số của ô và ô mìn có giá trị là 9
    //3) rows dòng tiếp theo : chứa trạng thái của ô
    //4) dòng tiếp theo      : deadCount 
    //5) deadCount dòng tiếp theo : chứa tọa độ x,y của ô có mìn đã đẳm



    //1) ---------------
    int rows,cols,mineCount,generated,ended,losed,countClick,countClickTB;
    float timeSave;
    if (!(input >> rows >> cols >> mineCount >> timeSave >> generated >> ended >> losed >> countClick >> countClickTB))
    { std::cout<<"vitri1"; return false; }

    // kiểm tra dữ liệu
    if (rows != this->rows || cols != this->cols || mineCount != this->mineCount || timeSave<0 || countClick <0 || countClick > countClickTB || countClickTB <0 || countClickTB>this->cols*this->rows-this->mineCount)
    { std::cout<<"vitri2"; return false; }

    auto isBool = [](int v) { return v == 0 || v == 1; }; // lambda
    if (!isBool(generated) || !isBool(ended) || !isBool(losed))
    { std::cout<<"vitri3"; return false;}

    // lưu tạm dữ liệu vào đây
    std::vector<Cell> loaded1(this->cells.size());

    //2) ---------------
    for (Cell& cell : loaded1)
    {
        int number;
        if (!(input >> number) || number <0 || number>9)
        { std::cout<<"vitri4"; return false; }
        
        if (number == 9)
        { cell.MineToggle(); }

        else { cell.SetNumber(number); }
    }

    //3) -----------------
    for (Cell& cell : loaded1)
    {
        int state;
        if (!(input >> state) || state <0 || state>3)
        { std::cout<<"vitri5"; return false; }
        switch(state)
        {
            case 0:
                cell.ChangeState(CellState::Openned);
                break;
            case 1:
                cell.ChangeState(CellState::Hidden);
                break;
            case 2:
                cell.ChangeState(CellState::Flagged);
                break;
            case 3:
                cell.ChangeState(CellState::Questioned);
                break;
        }
    }

    // kiểm tra dữ liệu
    int mines = 0;
    for (const Cell& cell : loaded1){ if (cell.CheckIsMine()) mines ++; }

    if (generated && mines != this->mineCount)
    { std::cout<<"vitri6"; return false; }
    
    if (!generated && mines != 0)
    { std::cout<<"vitri7"; return false;}


    //4) ---------------
    int deadCount;
    if (!(input >> deadCount) || deadCount<0 || deadCount >4)
    { std::cout<<"vitri8" <<deadCount ; return false; }

    // lưu tạm dữ liệu vào đây
    std::vector<CellPoint> loaded2;
    for (int i=0; i<deadCount; i++)
    {
        int x,y;
        if (!(input >> x >> y) || x<0 || y<0 || x>=this->cols || y>=this->rows)
        { return false; }
        loaded2.push_back(CellPoint(x,y));
    }

    /*Vùng xác nhận dữ liệu*/
    this->cells = std::move(loaded1);
    this->cellPointDead = std::move(loaded2);
    this->generated = generated;
    this->ended = ended;
    this->losed = losed;
    time = timeSave;

    return true;
}

void Board::Save(std::ostream& output, float time) const
{
    /* Mô tả file data */
    //1) dòng đầu tiên "rows cols minesCount timeSave generated ended losed clickCount clickCountTB"
    //2) rows dòng tiếp theo : chứa số của ô và ô mìn có giá trị là 9
    //3) rows dòng tiếp theo : chứa trạng thái của ô
    //4) dòng tiếp theo      : deadCount 
    //5) deadCount dòng tiếp theo : chứa tọa độ x,y của ô có mìn đã đẳm


    //1) ---------------
    output << this->rows << " " << this->cols << " " << this->mineCount << " " << time << " " << this->generated << " " << this->ended << " " << this->losed << " " << this->countClick << " " << this->countClickTB << "\n";

    //2) ---------------
    for (int y =0; y<this->rows; y++)
    {
        for (int x =0; x<this->cols; x++)
        {
            output << (this->cells[y*this->cols+x].CheckIsMine() ? 9 : this->cells[y*this->cols+x].GetNumber());
            output << (x<this->cols-1 ? " " : "\n");
        }
    }
    output << "\n";

    //3) ---------------
    for (int y =0; y<this->rows; y++)
    {
        for (int x =0; x<this->cols; x++)
        {
            switch (this->cells[y*this->cols+x].GetState())
            {
                case CellState::Openned:
                    output << 0;
                    break;
                case CellState::Hidden:
                    output << 1;
                    break;
                case CellState::Flagged:
                    output << 2;
                    break;
                case CellState::Questioned:
                    output << 3;
                    break;
            }
            output << (x<this->cols-1 ? " " : "\n");
        }
    }

    //4) --------------
    int deadCount = this->cellPointDead.size();
    output << deadCount <<"\n";

    //5) -------------
    for (const CellPoint& point : this->cellPointDead)
    {
        output << point.x << " " << point.y << "\n";
    }
}