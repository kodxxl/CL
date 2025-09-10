// TM With guidance of DeepSeek 09.2025

#include <iostream>
#include <utility>
#include <tuple>
#include <vector>
#include <stdexcept>
#include <memory>

class Symbol;

class State {
private:
    void* current;  
public:
    State() : current(nullptr) {}
    State(void* data) : current(data) {}
    
    void* get() const {
        return current;
    }
    void set(void* data){
        current = data;
    }
    bool Compare(const State& other) const {
        return current == other.current;
    }
    
    bool operator==(const State& other) const {
        return Compare(other);
    }
};

class Tape;

class Head {
private:
    State stateCurrent;
    Tape* tapeCurrent;
    int position;
public:
    Head() : tapeCurrent(nullptr), position(0) {}
    
    Symbol* getSymbol();
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

class Transition {
private:
    std::tuple<Symbol, State, bool> newState;
    std::pair<Symbol, State> current;
public:
    Transition() = delete;
    Transition(std::pair<Symbol, State> setCurrent, std::tuple<Symbol, State, bool> setNew) : 
        current(setCurrent),
        newState(setNew) {}
    
    std::tuple<Symbol, State, bool> make(const Symbol& sym, const State& state) const {
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
    const std::tuple<Symbol, State, bool>& getNewState() const { return newState; }
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
        // Если позиция за пределами ленты, добавляем пустые символы
        while (position >= static_cast<int>(tapeSymbols.size())) {
            tapeSymbols.push_back(Symbol(nullptr));
        }
        return &tapeSymbols[position];
    }
    
    void addSymbol(const Symbol& sym) {
        tapeSymbols.push_back(sym);
    }
    
    void setSymbol(int position, const Symbol& sym) {
        // Гарантируем, что позиция существует
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
                std::cout << "X ";
            }
        }
        std::cout << std::endl;
    }
};

Symbol* Head::getSymbol() {
    if (tapeCurrent) {
        return tapeCurrent->getSymbol(position);
    }
    return nullptr;
}

class TM {
private:
    Tape* myTape;
    std::vector<Transition> transitions;
    Head head;
    bool accepted;
    int maxSteps;

public:
    TM(Tape* tape, int initPointer) : myTape(tape), accepted(false), maxSteps(1000) {
        head.setTape(tape);
        head.setPosition(initPointer);
        // Устанавливаем начальное состояние
        head.setState(State(reinterpret_cast<void*>(0)));
    }
    
    void setInitialPosition(int pointer) {
        head.setPosition(pointer);
    }
    
    void addTransition(const std::pair<Symbol, State>& current, 
                      const std::tuple<Symbol, State, bool>& next) {
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
            
            for (const auto& transition : transitions) {
                if (transition.Compare(*currentSymbol, currentState)) {
                    auto [newSymbol, newState, moveLeft] = transition.make(*currentSymbol, currentState);
                    
                    // Применяем переход
                    myTape->setSymbol(head.getPosition(), newSymbol);
                    head.setState(newState);
                    head.move(moveLeft);
                    
                    transitionFound = true;
                    break;
                }
            }
            
            if (!transitionFound) {
                break;
            }
        }
        
        // Если достигли состояния без переходов, считаем принятым
        accepted = true;
        return myTape;
    }
    
    bool isAccepted() const { return accepted; } 
    
    void setMaxSteps(int steps) { maxSteps = steps; }
};

// Вспомогательные функции для создания состояний и символов
State createState(int id) {
    return State(reinterpret_cast<void*>(id));
}

Symbol createSymbol(int id) {
    return Symbol(reinterpret_cast<void*>(id));
}

int main() {
    // Создаем начальную ленту с некоторыми символами
    std::vector<Symbol> initialSymbols;
    for (int i = 0; i < 10; i++) {
        initialSymbols.push_back(createSymbol(0)); // Пустые символы
    }
    initialSymbols[5] = createSymbol(1); // Помечаем определенную позицию
    
    Tape tape(initialSymbols);
    
    TM t(&tape, 5);
    
    // Определяем состояния
    State state0 = createState(0); // Начальное состояние
    State state1 = createState(1); // Состояние обработки
    State state2 = createState(2); // Принимающее состояние
    
    // Определяем символы
    Symbol blank = createSymbol(0);
    Symbol mark = createSymbol(1);
    
    // Добавляем переходы
    // Из state0, если видим метку, меняем на пустой, двигаемся вправо, переходим в state1
    t.addTransition({mark, state0}, {blank, state1, false});
    
    // Из state1, если видим пустой, меняем на метку, двигаемся влево, переходим в state2 (принять)
    t.addTransition({blank, state1}, {mark, state2, true});
    
    std::cout << "Начальная лента: ";
    tape.display();
    
    // Запускаем машину Тьюринга
    Tape* result = t.run();
    
    std::cout << "Финальная лента: ";
    result->display();
    
    if (t.isAccepted()) {
        std::cout << "Вход принят!" << std::endl;
        
        // Создаем новую ленту для второй фазы
        std::vector<Symbol> newSymbols;
        for (int i = 0; i < 8; i++) {
            newSymbols.push_back(createSymbol(0));
        }
        Tape newTape(newSymbols);
        
        t.changeTape(&newTape);
        t.clearTransitions();
        t.setInitialPosition(3);
        
        // Добавляем новые переходы для второй фазы
        State state3 = createState(3);
        State state4 = createState(4);
        
        // Простые переходы для демонстрации
        t.addTransition({blank, state2}, {mark, state3, false});
        t.addTransition({blank, state3}, {mark, state4, true});
        
        std::cout << "Начальная лента второй фазы: ";
        newTape.display();
        
        Tape* secondResult = t.run();
        
        std::cout << "Финальная лента второй фазы: ";
        secondResult->display();
    }
    
    if (t.isAccepted()) {
        std::cout << "Финальный результат: Принято" << std::endl;
        return 1;
    } else {
        std::cout << "Финальный результат: Отклонено" << std::endl;
        return 0;
    }
}
