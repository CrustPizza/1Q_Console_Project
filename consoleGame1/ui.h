#pragma once

void cursor_changer();
void TitleDraw();
void MenuDraw(const int, const int , const int);
void ModeListDraw(const int, const int ,const int);
void InfoDraw(const int , const int );
bool OptionDraw(const int x, const int y, const int select, bool flag);
void MultiDraw(const int x, const int y, const int select);
void HostRoom(const int x, const int y, const shared_ptr<Server> host);
void UserRoom(const int x, const int y, const int select, const string input, const bool start, bool& is_error);
void EndingDraw(int winer);