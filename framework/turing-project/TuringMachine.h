#ifndef __TURING_MACHINE__
#define __TURING_MACHINE__

#include<bits/stdc++.h>

using namespace std;

std::string String(char x, int num) {
    std::string s;
    for(int i = 0; i < num; ++i) s += x;
    return s;
}

class TuringMachine { // Turing machine
private:
    map<string, int> state_id;
    set<char> Tape_Chars, Input_Chars;
    bool _is_built;
    int N, N_States, q0;

    class _State {
    public:
        string name;
        int id;
        bool is_final;
        vector< pair< string, 
                    pair< pair<string,string>, 
                          int > > > trans; // transition methods
        _State(std::string s, int _id) : name(s), id(_id), is_final(0) {}
    };
    vector<_State> State;

    /**** string comparation ****/
    bool match(std::string s, std::string t) { // if s matches t
        if(s.size() != t.size()) return 0;
        for(int i = 0; i < s.size(); ++i) {
            if(s[i] != t[i] && (t[i] != '*' || s[i] == '_')) return 0;
        }
        return 1;
    }
    std::string get_trans(std::string s, std::string t) { // convert s to t
        for(int i = 0; i < s.size(); ++i) {
            if(t[i] == '*') t[i] = s[i];
        }
        return t;
    }
    /****************************/

    bool self_checked_state() { // return 1 : ok, 0 : wrong
        for(char x : Input_Chars) {
            if(!Tape_Chars.count(x)) {
                // std::cerr << "Error : " << x << "is in Input Chars but Not in Tape Chars.\n";
                return 0;
            }
        }

        for(int i = 0; i < N; ++i) {
            for(int j = 0; j < State[i].trans.size(); ++j) {
                for(int k = 0; k < N; ++k) {
                    if(State[i].trans[j].second.first.first[k] == '*' && State[i].trans[j].first[k] != '*') {
                        // std::cerr << "Error : The transition of State " << State[i].name << " : "
                        //  << State[i].trans[j].first << "->" << State[i].trans[j].second.first.first << " is wrong\n";
                        return 0;
                    }
                }
            }
            for(int j = 0; j < State[i].trans.size(); ++j)
                for(int k = 0; k < State[i].trans.size(); ++k) if(j != k) {
                    if(match(State[i].trans[j].first, State[i].trans[k].first)) {
                        // std::cerr << "Error : The transition function confuse of State " << State[i].name << "\n";
                        return 0;
                    }
                }
        }

        return 1;
    }
    bool self_checked_input(std::string s, bool verbose) { // return 1 : ok, 0 : wrong
        for(int i = 0; i < s.size(); ++i) if(!Input_Chars.count(s[i])) {
            if(!verbose) {
                std::cerr << "illegal input string\n";
            } else {
                std::cerr << "Input: " << s << "\n";
                std::cerr << "==================== ERR ====================\n";
                std::cerr << "error: Symbol \"" << s[i] << "\" in input is not defined in the set of input symbols\n";
                std::cerr << "Input: " << s << "\n";
                std::cerr << "       " << String(' ', i) << "^" << "\n";
                std::cerr << "==================== END ====================\n";
            }
            return 0;
        }
        return 1;
    }
    class Tape {
        vector<char> l, r; // l : index negative, r : index non-negative
        int posl, posr; // the left and right side of the tape
        pair<bool, int> Hd; // the head position

        bool Greater(pair<bool, int> pos) { // if hd be the right of pos
            if(Hd.first > pos.first) return 1;
            if(Hd.first) {
                return Hd.second > pos.second;
            }
            return Hd.second < pos.second;
        }
        bool Less(pair<bool, int> pos) { // if hd be the left of pos
            if(Hd.first < pos.first) return 1;
            if(Hd.first) {
                return Hd.second < pos.second;
            }
            return Hd.second > pos.second;
        }

        void Shrink() { // ignore the blank chars of two sides
            while(l.size() && l.back() == '_' && Greater({0, posl})) --posl, l.pop_back();
            while(r.size() && r.back() == '_' && Less({1, posr}) && (posl >= 0 || posr >= 1)) --posr, r.pop_back();
        }
        int GetHdPos() {
            if(Hd.first) return Hd.second;
            return -1-Hd.second;
        }
        public:
        void MoveRight() {
            if(Hd.first) {
                if(Hd.second < posr) ++ Hd.second;
                else {
                    r.push_back('_');
                    ++posr;
                    ++ Hd.second;
                }
            } else {
                if(Hd.second == 0) {
                    if(r.size() == 0) r.push_back('_'), ++posr;
                    Hd = {1, 0};
                } else {
                    Hd.second --;
                }
            }
            Shrink();
        }
        void MoveLeft() {
            if(Hd.first) {
                if(Hd.second == 0) l.push_back('_'), ++posl, Hd = {0, 0};
                else Hd.second--;
            } else {
                if(Hd.second < posl) ++ Hd.second;
                else {
                    l.push_back('_');
                    ++posl;
                    ++Hd.second;
                }
            }
            Shrink();
        }
        Tape() : posl(-1), posr(0), Hd({1, 0}), r({'_'}), l(0) {}
        Tape(string s) {
            if(s == "") {
                posr = 0;
                posl = -1;
                r = {'_'};
                Hd = {1, 0};
                return ;
            }
            posl = -1;
            posr = s.size() - 1;
            r.resize(s.size());
            for(int i = 0; i <= posr; ++i) r[i] = s[i];
            Hd = {1, 0};
        }
        void print(int prelen, int index) { // print situation of the tape
            string _Index = "Index" + to_string(index) + String(' ', prelen - 5 - to_string(index).size()) + ":";
            string _Tape  = "Tape"  + to_string(index) + String(' ', prelen - 4 - to_string(index).size()) + ":";
            string _Head  = "Head"  + to_string(index) + String(' ', prelen - 4 - to_string(index).size()) + ":";

            Shrink();

            int L = 0, R = 0;
            if(posr < 0) while(L < l.size() && l[L] == '_' && Hd.second > L) ++L;
            if(posl < 0) while(R < r.size() && r[R] == '_' && Hd.second > R) ++R;

            for(int i = posl; i >= L; --i) {
                int id = -1-i;
                string _i = to_string(abs(id));
                string _t = String(l[i], 1);
                string _h = GetHdPos() == id ? "^" : " ";

                int len = max(_i.size(), max(_t.size(), _h.size()));
                _Index += " " + _i + String(' ', len - _i.size());
                _Tape  += " " + _t + String(' ', len - _t.size());
                _Head  += " " + _h + String(' ', len - _h.size());
            }
            for(int i = R; i <= posr; ++i) {
                int id = i;
                string _i = to_string(abs(id));
                string _t = String(r[i], 1);
                string _h = GetHdPos() == id ? "^" : " ";

                int len = max(_i.size(), max(_t.size(), _h.size()));
                _Index += " " + _i + String(' ', len - _i.size());
                _Tape  += " " + _t + String(' ', len - _t.size());
                _Head  += " " + _h + String(' ', len - _h.size());
            }

            while(_Head.back() == ' ') _Head.pop_back();
            std::cout << _Index << "\n" << _Tape << "\n" << _Head << "\n";
        }
        char GetHdChar() {
            if(Hd.first) return r[Hd.second];
            return l[Hd.second];
        }
        std::string GetString() {
            std::string ret = "";
            Shrink();

            int L = 0, R = 0;
            if(posr < 0) while(L < l.size() && (l[L] == '_' || l[L] == ' ') && Hd.second > L) ++L;
            if(posl < 0) while(R < r.size() && (r[R] == '_' || r[R] == ' ') && Hd.second > R) ++R;

            for(int i = posl, j = 0; i >= L; --i) {
                if((l[i] == '_' || l[i] == ' ') && !j) continue;
                else {
                    j = 1;
                    ret +=  l[i];
                }
            }
            int RR = posr; while(RR >= 0 && (r[RR] == '_' || r[RR] == ' ')) --RR;
            for(int i = R; i <= RR; ++i) ret += r[i];
            return ret;
        }
        void ChangeHdChar(char x) {
            if(Hd.first) {r[Hd.second] = x;}
            else l[Hd.second] = x;
        }
    };
public:
    TuringMachine () : _is_built(0) {}
    TuringMachine (std::string s) : _is_built(0) {
        if(!this->SetupMachine(s)) exit(-1);
    }
    bool Run(std::string s = "", bool verbose = 0) { // return 1 : ok, 0 : meet error
        if(!_is_built) {
            std::cerr << "Error : Turing Machine is not defined.\n";
            return 0;
        }
        if(!self_checked_input(s, verbose)) {
            return 0;
        }
        if(verbose) {
            std::cout << "Input: " << s << "\n";
            std::cout << "==================== RUN ====================\n";
        }
        int step = -1, now = q0;
        int prelen = 5 + to_string(N).size() + 1;
        bool Acc; std::string _Acc[2] = {"No", "Yes"}, __Acc[2] = {"UNACCEPTED", "ACCEPTED"};

        vector<Tape> tapes(N); tapes[0] = Tape(s);

        do {
            ++step;
            Acc |= State[now].is_final;
            if(verbose) {
                std::cout << "Step" << String(' ', prelen - 4) << ": " << step << "\n";
                std::cout << "State" << String(' ', prelen - 5) << ": " << State[now].name << "\n";
                std::cout << "Acc" << String(' ', prelen - 3) << ": " << _Acc[Acc] << "\n";
                for(int i = 0; i < N; ++i) tapes[i].print(prelen, i);
                std::cout << "---------------------------------------------\n";
            }

            std::string s;
            for(int i = 0; i < N; ++i) s += tapes[i].GetHdChar();
            int next = -1;

            for(auto &pair : State[now].trans) { // find next state
                auto &x = pair.first;
                auto &y = pair.second;
                if(match(s, x)) {
                    next = y.second;
                    s = get_trans(s, y.first.first);
                    for(int i = 0; i < N; ++i) {
                        tapes[i].ChangeHdChar(s[i]);
                        if(y.first.second[i] == 'l') tapes[i].MoveLeft();
                        if(y.first.second[i] == 'r') tapes[i].MoveRight();
                    }
                    break;
                }
            }

            now = next;

        } while(now != -1);

        if(verbose) {
            std::cout << __Acc[Acc] << "\nResult: " << tapes[0].GetString() << "\n";
            std::cout << "==================== END ====================\n";
        } else {
            std::cout << "(" << __Acc[Acc] << ") " << tapes[0].GetString() << "\n";
        }

        return 1;
    }

    bool SetupMachine(std::string filename) { // return 1 : ok, 0 : wrong
        #define SYNTAX_ERROR {\
                    std::cerr << "syntax error\n"; \
                    return 0; \
                    }\
        
        std::ifstream IN(filename);
        if(!IN.is_open()) {
            // std::cerr << "Error : No such file " << filename << "\n";
            SYNTAX_ERROR;
            return 0;
        }
        State.clear(); Tape_Chars.clear(); Input_Chars.clear();
        N = N_States = 0;  state_id.clear(); _is_built = 0;

        std::string line;

        // Q
        {
        bool read_Q = 0;
        while(getline(IN, line)) {
            for(int i = 0; i < line.size(); ++i) {
                if(line[i] == ';') break;
                else if(line[i] == ' ') continue;
                else if(line[i] == '#' && i + 1 < line.size() && line[i + 1] == 'Q') {
                    int j;
                    for(j = i + 2; j < line.size() && line[j] == ' '; ++j);
                    if(j >= line.size() || line[j] != '=') SYNTAX_ERROR;
                    for(++j; j < line.size() && line[j] == ' '; ++j);
                    if(j >= line.size() || line[j] != '{') SYNTAX_ERROR;

                    string tmp = "";
                    i = j + 1;
                    for(; i < line.size(); ++i) {
                        if(line[i] == ',') {
                            if(tmp.size() == 0) SYNTAX_ERROR;
                            state_id[tmp] = N_States++;
                            State.push_back(_State(tmp, N_States - 1));
                            tmp = "";
                        }
                        else if(line[i] == '}') {
                            if(tmp.size() == 0) SYNTAX_ERROR;
                            state_id[tmp] = N_States++;
                            State.push_back(_State(tmp, N_States - 1));
                            tmp = "";
                            break;
                        } else if(line[i] != ' ') {
                            if(line[i] == '_' || isdigit(line[i]) || isalpha(line[i])) tmp = tmp + line[i];
                            else SYNTAX_ERROR;
                        }
                    }
                    if(i >= line.size() || line[i] != '}') SYNTAX_ERROR;
                    for(++i ; i < line.size(); ++i) {
                        if(line[i] == ';') break;
                        else if(line[i] != ' ') SYNTAX_ERROR;
                    }
                    read_Q = 1;
                }
                else SYNTAX_ERROR;
            }
            if(read_Q) break;
        }

        if(!read_Q) SYNTAX_ERROR;
        }
        // S
        {
        bool read_S = 0;
        while(getline(IN, line)) {
            for(int i = 0; i < line.size(); ++i) {
                if(line[i] == ';') break;
                else if(line[i] == ' ') continue;
                else if(line[i] == '#' && i + 1 < line.size() && line[i + 1] == 'S') {
                    int j;
                    for(j = i + 2; j < line.size() && line[j] == ' '; ++j);
                    if(j >= line.size() || line[j] != '=') SYNTAX_ERROR;
                    for(++j; j < line.size() && line[j] == ' '; ++j);
                    if(j >= line.size() || line[j] != '{') SYNTAX_ERROR;

                    string tmp = "";
                    i = j + 1;
                    for(; i < line.size(); ++i) {
                        if(line[i] == ',') {
                            if(tmp.size() != 1) SYNTAX_ERROR;
                            Input_Chars.insert(tmp[0]);
                            tmp = "";
                        }
                        else if(line[i] == '}') {
                            if(tmp.size() != 1) SYNTAX_ERROR;
                            Input_Chars.insert(tmp[0]);
                            tmp = "";
                            break;
                        } else if(line[i] != ' ') {
                            tmp = tmp + line[i];
                        }
                    }
                    if(i >= line.size() || line[i] != '}') SYNTAX_ERROR;
                    for(++i ; i < line.size(); ++i) {
                        if(line[i] == ';') break;
                        else if(line[i] != ' ') SYNTAX_ERROR;
                    }
                    read_S = 1;
                }
                else SYNTAX_ERROR;
            }
            if(read_S) break;
        }
        if(!read_S) SYNTAX_ERROR;
        }
        // G
        {
        bool read_G = 0;
        while(getline(IN, line)) {
            for(int i = 0; i < line.size(); ++i) {
                if(line[i] == ';') break;
                else if(line[i] == ' ') continue;
                else if(line[i] == '#' && i + 1 < line.size() && line[i + 1] == 'G') {
                    int j;
                    for(j = i + 2; j < line.size() && line[j] == ' '; ++j);
                    if(j >= line.size() || line[j] != '=') SYNTAX_ERROR;
                    for(++j; j < line.size() && line[j] == ' '; ++j);
                    if(j >= line.size() || line[j] != '{') SYNTAX_ERROR;

                    string tmp = "";
                    i = j + 1;
                    for(; i < line.size(); ++i) {
                        if(line[i] == ',') {
                            if(tmp.size() != 1) SYNTAX_ERROR;
                            Tape_Chars.insert(tmp[0]);
                            tmp = "";
                        }
                        else if(line[i] == '}') {
                            if(tmp.size() != 1) SYNTAX_ERROR;
                            Tape_Chars.insert(tmp[0]);
                            tmp = "";
                            break;
                        } else if(line[i] != ' ') {
                            tmp = tmp + line[i];
                        }
                    }
                    if(i >= line.size() || line[i] != '}') SYNTAX_ERROR;
                    for(++i ; i < line.size(); ++i) {
                        if(line[i] == ';') break;
                        else if(line[i] != ' ') SYNTAX_ERROR;
                    }
                    read_G = 1;
                }
                else SYNTAX_ERROR;
            }
            if(read_G) break;
        }
        if(!read_G) SYNTAX_ERROR;
        if(Tape_Chars.count('_') == 0) {
            // std::cerr << "Error : \'_\' not be in the Tape Chars.\n";
            SYNTAX_ERROR;
        }
        if(Tape_Chars.count('*')) {
            // std::cerr << "Error : '*' be in the Tape Chars.\n";
            SYNTAX_ERROR;
        }
        }
        // q0
        {
        bool read_q0 = 0;
        while(getline(IN, line)) {
            for(int i = 0; i < line.size(); ++i) {
                if(line[i] == ';') break;
                else if(line[i] == ' ') continue;
                else if(line[i] == '#' && i + 1 < line.size() && line[i + 1] == 'q' && i + 2 < line.size() && line[i + 2] == '0') {
                    int j;
                    for(j = i + 3; j < line.size() && line[j] == ' '; ++j);
                    if(j >= line.size() || line[j] != '=') SYNTAX_ERROR;

                    string tmp = "";
                    i = j + 1;
                    for(; i < line.size(); ++i) {
                        if(line[i] != ' ') {
                            tmp = tmp + line[i];
                        }
                        if(line[i] == ';') break;
                    }
                    if(!state_id.count(tmp)) SYNTAX_ERROR;
                    q0 = state_id[tmp];
                    read_q0 = 1;
                }
                else SYNTAX_ERROR;
            }
            if(read_q0) break;
        }
        if(!read_q0) SYNTAX_ERROR;
        }
        // B
        {
        bool read_B = 0;
        while(getline(IN, line)) {
            for(int i = 0; i < line.size(); ++i) {
                if(line[i] == ';') break;
                else if(line[i] == ' ') continue;
                else if(line[i] == '#' && i + 1 < line.size() && line[i + 1] == 'B') {
                    int j;
                    for(j = i + 2; j < line.size() && line[j] == ' '; ++j);
                    if(j >= line.size() || line[j] != '=') SYNTAX_ERROR;

                    string tmp = "";
                    i = j + 1;
                    for(; i < line.size(); ++i) {
                        if(line[i] != ' ') {
                            tmp = tmp + line[i];
                        }
                        if(line[i] == ';') break;
                    }
                    if(tmp != "_") SYNTAX_ERROR;
                    read_B = 1;
                }
                else SYNTAX_ERROR;
            }
            if(read_B) break;
        }
        if(!read_B) SYNTAX_ERROR;
        }
        // F
        {
        bool read_F = 0;
        while(getline(IN, line)) {
            for(int i = 0; i < line.size(); ++i) {
                if(line[i] == ';') break;
                else if(line[i] == ' ') continue;
                else if(line[i] == '#' && i + 1 < line.size() && line[i + 1] == 'F') {
                    int j;
                    for(j = i + 2; j < line.size() && line[j] == ' '; ++j);
                    if(j >= line.size() || line[j] != '=') SYNTAX_ERROR;
                    for(++j; j < line.size() && line[j] == ' '; ++j);
                    if(j >= line.size() || line[j] != '{') SYNTAX_ERROR;

                    string tmp = "";
                    i = j + 1;
                    for(; i < line.size(); ++i) {
                        if(line[i] == ',') {
                            if(tmp.size() == 0) SYNTAX_ERROR;
                            if(!state_id.count(tmp)) {
                                // std::cerr << "Final State : " << tmp << " Not found in States\n";
                                SYNTAX_ERROR;
                            }
                            State[state_id[tmp]].is_final = 1;
                            tmp = "";
                        }
                        else if(line[i] == '}') {
                            if(tmp.size() == 0) SYNTAX_ERROR;
                            if(!state_id.count(tmp)) {
                                // std::cerr << "Final State : " << tmp << " Not found in States\n";
                                SYNTAX_ERROR;
                            }
                            State[state_id[tmp]].is_final = 1;
                            tmp = "";
                            break;
                        } else if(line[i] != ' ') {
                            if(line[i] == '_' || isdigit(line[i]) || isalpha(line[i])) tmp = tmp + line[i];
                            else SYNTAX_ERROR;
                        }
                    }
                    if(i >= line.size() || line[i] != '}') SYNTAX_ERROR;
                    for(++i ; i < line.size(); ++i) {
                        if(line[i] == ';') break;
                        else if(line[i] != ' ') SYNTAX_ERROR;
                    }
                    read_F = 1;
                }
                else SYNTAX_ERROR;
            }
            if(read_F) break;
        }
        if(!read_F) SYNTAX_ERROR;
        }
        // N
        {
        bool read_N = 0;
        while(getline(IN, line)) {
            for(int i = 0; i < line.size(); ++i) {
                if(line[i] == ';') break;
                else if(line[i] == ' ') continue;
                else if(line[i] == '#' && i + 1 < line.size() && line[i + 1] == 'N') {
                    int j;
                    for(j = i + 2; j < line.size() && line[j] == ' '; ++j);
                    if(j >= line.size() || line[j] != '=') SYNTAX_ERROR;

                    string tmp = "";
                    i = j + 1;
                    for(; i < line.size(); ++i) {
                        if(line[i] != ' ') {
                            tmp = tmp + line[i];
                        }
                        if(line[i] == ';') break;
                    }
                    for(auto x : tmp) if(!isdigit(x)) SYNTAX_ERROR;
                    int num = 0;
                    for(auto x : tmp) num = num * 10 + x - '0';
                    if(num <= 0) {
                        // std::cerr << "N = " << num << ": Out of range\n";
                        SYNTAX_ERROR;
                    }
                    N = num;
                    read_N = 1;
                }
                else SYNTAX_ERROR;
            }
            if(read_N) break;
        }
        if(!read_N) SYNTAX_ERROR;
        }
        // trans
        while(getline(IN, line)) {
            stringstream STR(line);
            vector<string> ss;
            string word;
            while((STR>>word)) {
                if(word[0] == ';') {
                    break;
                }
                ss.push_back(word);
            }
            if(ss.size() >= 5) {
                if(ss.size() > 5) SYNTAX_ERROR;
                if(!state_id.count(ss[0])) SYNTAX_ERROR;
                if(!state_id.count(ss[4])) SYNTAX_ERROR;
                if(ss[1].size() != N || ss[2].size() != N || ss[3].size() != N) SYNTAX_ERROR;
                for(auto x : ss[1]) if(x!='*' && !Tape_Chars.count(x)) SYNTAX_ERROR;
                for(auto x : ss[2]) if(x!='*' && !Tape_Chars.count(x)) SYNTAX_ERROR;
                for(auto x : ss[3]) if(x!='*' && x!='l' && x!='r') SYNTAX_ERROR;
                State[state_id[ss[0]]].trans.push_back({ss[1], {{ss[2], ss[3]}, state_id[ss[4]]}});
            }
        }
        IN.close();
        if(!self_checked_state()) {
            // std::cerr << "Error : Setup Turing Machine from file " << filename << "\n";
            // return 0;
            SYNTAX_ERROR;
        }
        _is_built = 1;
        return 1;
    }
};

#endif