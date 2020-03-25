class AbstractStateObject
{
private:
    List<double> State { get; set; }
    List<ODE> ODEs { get; }
    ISolver Solver { get; set; }

public:
    AbstractStateObject(/* args */);
    ~AbstractStateObject();

    public void ComputeNextState(double step);
    public void OperationAfterSolve();
    public delegate double ODE(List<double> state);
};

AbstractStateObject::AbstractStateObject(/* args */)
{
}

AbstractStateObject::~AbstractStateObject()
{
}
