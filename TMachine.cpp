#include <iostream>
#include <tuple>
#include <pair>
#include <vector>

class State {
  private:
    void* current;  
  public:
    void* get(){
        return current;
    }
    void set(void* data){
        current = data;
    }
    bool Compare(State)
    {
        //realize correct comparation
    }
};

class Head {
  private:
    State stateCurrent;
    Tape* tapeCurrent;
    int position;
  public:
    Symbol* getSymbol(Tape* t){
        return
    }
    void setTape(){}
    void move(bool left){
        if(left)
        {
            
        }
        else{
            
        }
    }
};

class Symbol {
  private:
    void* current;
  public:
  Symbol(void* data) : current = data {}
   void* get(){return current;}
   void set(void* data)(current = data){}
   bool Compare(State)
    {
        //realize correct comparation
    }
};

calss Transition{
  private:
    tuple<Symbol, State, bool> newState;
    pair<Symbol, State> current;
  public:
    Transition() = delete;
    Transition(pair<Symbol, State> setCurrent, tuple<Symbol, State, bool/*direction*/> setNew) : 
      current = setCurrent;
      newState = setNew;
    make(Symbol , State){
        if Compare(Symbol , State))
        return newState;
        else throw false;
    }
    bool Compare(Symbol , State){
        if(current.first.Compare() && current.second.compare())
          return true;
        else return false;
    }
};

class Tape {
  private:
    vector<Symbol> tapeSymbols;
  public:
    Tape() = delete;
    Tape(vector<Symbol> symbols) : tapeSymbols = {}
    Symbol* getSymbol(int position){ return tapeSymbols[position]}
    void addSymbol(Symbol){
        
    };
};

class TM {
    Tape* myTape;
    vector<Transision> tansisions;
    bool accepted;
  public:
    TM(Tape* tape, int initPointer) myTape : tape, tapePointer = initPointer { accepted = false; }
    void addTransision(pair<Symbol, State>, tuple<Symbol, State, bool>){
        tansisions.push(Transision(pair<Symbol, State>, tuple<Symbol, State, bool>))
    }
    void setInitialPosition(int poiter)
    Tape run(Tape Input)
    {
        // read tape, make work and return outpuitTape
        return outTape;
    }
    bool isAccepted(){ return accepted} 
};

int main()
{
    
    TM(new Tape(), 7);

    return 0;
}
