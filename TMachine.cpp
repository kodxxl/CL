#include <iostream>
#include <utility>
#include <tuple>
#include <vector>
#include <stdexcept>
#include <memory>

class State {
  private:
    void* current;
    bool accepted = false;
  public:
    State() : current(nullptr) {}
    State(void* data, bool accept) : current(data), accepted(accept) {}
    
    void* get() const {
        return current;
    }
    void set(void* data){
        current = data;
    }
    bool Compare(const State& other) const {
        return current == other.current;
    }
    bool isAccepted(){
        return accepted;
    }
    bool operator==(const State& other) const {
        return Compare(other);
    }
};

class Symbol {
  private:
    void* current;
  public:
    Symbol() : current(nullptr) {}
    Symbol(void* data) : current(data) {}
    
    void* get() const { return current; }
    void set(void* data) { current = data; }
    bool Compare(const Symbol& other) const {
        return current == other.current;
    }
    
    bool operator==(const Symbol& other) const {
        return Compare(other);
    }
};

class Tape {
  private:
    std::vector<Symbol> tapeSymbols;
  public:
    Tape() : tapeSymbols() {}
    Tape(const std::vector<Symbol>& symbols) : tapeSymbols(symbols) {}
    
    Symbol* getSymbol(int position) { 
        if (position >= 0 && position < static_cast<int>(tapeSymbols.size())) {
            return &tapeSymbols[position];
        }
        while (position >= static_cast<int>(tapeSymbols.size())) {
            tapeSymbols.push_back(Symbol(nullptr));
        }
        return &tapeSymbols[position];
    }
    
    void addSymbol(const Symbol& sym) {
        tapeSymbols.push_back(sym);
    }
    
    void setSymbol(int position, const Symbol& sym) {
        if (position >= static_cast<int>(tapeSymbols.size())) {
            tapeSymbols.resize(position + 1, Symbol(nullptr));
        }
        tapeSymbols[position] = sym;
    }
    
    size_t size() const { return tapeSymbols.size(); }
    
    void display() const {
        for (const auto& sym : tapeSymbols) {
            if (sym.get() == nullptr) {
                std::cout << "_ ";
            } else {
                std::cout << sym.get();
            }
        }
        std::cout << std::endl;
    }
};

class Head {
  private:
    State stateCurrent;
    Tape* tapeCurrent;
    int position;
  public:
    Head() : tapeCurrent(nullptr), position(0) {}
    
    void setTape(Tape* tape) {
        tapeCurrent = tape;
    }
    void move(bool left){
        if(left) {
            position--;
        } else {
            position++;
        }
    }
    int getPosition() const { return position; }
    void setPosition(int pos) { position = pos; }
    State getState() const { return stateCurrent; }
    void setState(const State& state) { stateCurrent = state; }
    Symbol* getSymbol() {
      if (tapeCurrent) {
        return tapeCurrent->getSymbol(position);
      }
      return nullptr;
    }

};

class Transition {
  private:
    std::tuple<Symbol, State, bool, bool> newState;
    std::pair<Symbol, State> current;
    bool stop = false;
  public:
    Transition() = delete;
    Transition(std::pair<Symbol, State> setCurrent, std::tuple<Symbol, State, bool, bool> setNew) : 
        current(setCurrent),
        newState(setNew) {}
    
    std::tuple<Symbol, State, bool, bool> make(const Symbol& sym, const State& state) const {
        if (Compare(sym, state)) {
            return newState;
        } else {
            throw std::runtime_error("Transition not applicable");
        }
    }
    
    bool Compare(const Symbol& sym, const State& state) const {
        return (current.first.Compare(sym) && current.second.Compare(state));
    }
    
    const std::pair<Symbol, State>& getCurrent() const { return current; }
    const std::tuple<Symbol, State, bool, bool>& getNewState() const { return newState; }
};

class TM {
  private:
    Tape* myTape;
    std::vector<Transition> transitions;
    Head head;
    bool accepted = false;
    int maxSteps;

  public:
    TM(Tape* tape, int initPointer) : myTape(tape), accepted(false), maxSteps(1000) {
        head.setTape(tape);
        head.setPosition(initPointer);
    }
    
    void setInitialPosition(int pointer) {
        head.setPosition(pointer);
    }
    
    void addTransition(const std::pair<Symbol, State>& current, 
                      const std::tuple<Symbol, State, bool, bool>& next) {
        transitions.emplace_back(current, next);
    }
    
    void clearTransitions() {
        transitions.clear();
    }
    
    void changeTape(Tape* newTape) {
        myTape = newTape;
        head.setTape(newTape);
    }
    
    Tape* run() {
        int steps = 0;
        while (steps < maxSteps) {
            steps++;
            
            Symbol* currentSymbol = head.getSymbol();
            State currentState = head.getState();
            bool transitionFound = false;
            bool stop = false;
            
            for (const auto& transition : transitions) {
                if (transition.Compare(*currentSymbol, currentState)) {
                    auto [newSymbol, newState, moveDirection, stop] = transition.make(*currentSymbol, currentState);
                    
                    myTape->setSymbol(head.getPosition(), newSymbol);
                    head.setState(newState);
                    head.move(moveDirection);
                    
                    if(newState.isAccepted()){
                        accepted = true;
                        return myTape;
                    }
                    if(stop) return myTape;
                    
                    transitionFound = true;
                    break;
                }
            }
            if (!transitionFound) {
                break;
            }
        }
        return myTape;
    }
    
    bool isAccepted() const { return accepted; } 
    
    void setMaxSteps(int steps) { maxSteps = steps; }
};

// int to void ---
State createState(int id, bool accept = false) {
    return State(reinterpret_cast<void*>(id), accept);
}

Symbol createSymbol(int id) {
    return Symbol(reinterpret_cast<void*>(id));
}
void DisplayAcceptance(bool state){
    if (state) {
        std::cout << "Final result: Accepted" << std::endl;
    } else {
        std::cout << "Final result: Not Accepted" << std::endl;
    }
}
//-------------

int main() {
    std::vector<Symbol> initialSymbols;
    Symbol blank = createSymbol(0);
    Symbol mark = createSymbol(1);
    Symbol tree = createSymbol(3);
    
    initialSymbols.push_back(mark);
    for (int i = 0; i < 10; i++) {
        initialSymbols.push_back(blank);
    }
    initialSymbols[5] = mark;
    
    Tape tape(initialSymbols);
    
    TM t(&tape, 5);
    
    const bool left = false;
    const bool right = true;
    const bool stop = true;
    const bool accepted = true;
    
    State state0 = createState(0);
    State state1 = createState(1);
    State state2 = createState(2);
    
    t.addTransition({mark, state0}, {blank, state1, left, not stop});
    t.addTransition({blank, state1}, {mark, state2, right, not stop});
    t.addTransition({blank, state2}, {tree, state2, right, not stop});
    t.addTransition({mark, state2}, {blank, state2, right, not stop});
    
    std::cout << "Initial tape: ";
    tape.display();
    
    Tape* result = t.run();
    
    std::cout << "Final tape: ";
    result->display();
    
    DisplayAcceptance(t.isAccepted());
    
    if (! t.isAccepted()) {
        std::vector<Symbol> newSymbols;
        for (int i = 0; i < 8; i++) {
            newSymbols.push_back(blank);
        }
        Tape newTape(newSymbols);
        
        t.changeTape(&newTape);
        t.clearTransitions();
        t.setInitialPosition(3);
        
        State state3 = createState(3, accepted);
        State state4 = createState(4);
        
        t.addTransition({blank, state2}, {mark, state3, left, not stop});
        t.addTransition({blank, state3}, {mark, state4, right, not stop});
        
        std::cout << "Second phase initial tape: ";
        newTape.display();
        
        Tape* secondResult = t.run();
        
        std::cout << "Second phase final tape: ";
        secondResult->display();
    }
    
    DisplayAcceptance(t.isAccepted());
}
