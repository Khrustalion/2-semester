#pragma once
#include <memory>
#include <vector>
#include <iostream>

class BaseHolder {
    public:
    
    virtual ~BaseHolder() = default;
};

template<typename T>
class Holder : public BaseHolder {
    private:
    T value_;
    
    public:
    Holder() = default;

    Holder(const T& value) : value_{value} {}
    
    T Get() {
        return value_;
    }
};

class Any {
    private:
    std::shared_ptr<BaseHolder> holder_;
    
    public:
    ~Any() = default;

    Any() = default;

    template <typename T>
    Any(const T& value) {
        holder_ = std::make_shared<Holder<T>>(value);
    }  

    template <typename T>
    void Set(const T& value) {
        holder_ = std::make_shared<Holder<T>>(value);
    }

    template <typename T>
    Any operator=(const T& value) {
        holder_ = std::make_shared<Holder<T>>(value);

        return *this;
    }
    
    template<typename T>
    T Get() {
        Holder<T>* new_holder = dynamic_cast<Holder<T>*>(holder_.get());
        return new_holder->Get();
    }
};

class BaseTask {
    public:
    virtual ~BaseTask() = default;

    virtual Any getResult() = 0;
};

template <typename T, typename t1>
class Task1Argument : public BaseTask {
    private:
    t1 task_;
    T task_func_;
    Any current_value_;

    public:
    ~Task1Argument() = default;

    Task1Argument() = default;

    Task1Argument(const Task1Argument& task) : task_func_{task.task_func_},
                                               task_{task.task_}
                                               {}

    Task1Argument(const T& task_func, const t1& task) : task_func_{task_func},
                                                        task_{task}
                                                        {}

    Any getResult() {
        current_value_ = task_func_(task_.getResult());
        return current_value_;
    }
};

template<typename T, typename t1, typename t2>
class Task2Argument : public BaseTask {
    private:
    t1 task_1_;
    t2 task_2_;
    T task_func_;
    Any current_value_;

    public:
    ~Task2Argument() = default;

    Task2Argument() = default;

    Task2Argument(const Task2Argument& task) : task_1_{task.task_1_},
                                               task_2_{task.task_2_},
                                               task_func_{task.task_func_} {}

    Task2Argument(const T& new_task_func, const t1& new_task_1, const t2& new_task_2) : task_func_{new_task_func} {
        task_1_ = new_task_1;
        task_2_ = new_task_2;
    }

    Any getResult() {
        current_value_ = task_func_(task_1_.getResult(), task_2_.getResult());
        return current_value_;
    }
};

class Task {
    private:
    std::shared_ptr<BaseTask> task_;
    Any result_;
    bool need_calculated_;

    public:
    ~Task() = default;

    Task() = default;

    template <typename T, typename t1, typename t2>
    Task(const T& task_func, const t1&  task_1, const t2& task_2) : task_{new Task2Argument(task_func, task_1, task_2)} {
        need_calculated_ = true;
    }

    template <typename T, typename t1>
    Task(const T& task_func, const t1& task_1) : task_{new Task1Argument(task_func, task_1)} {
        need_calculated_ = true;
    }

    template<typename t>
    Task(const t& value) {
        result_ = value;
        need_calculated_ = false;
    }

    bool f() {
        return need_calculated_;
    }

    void caclulate() {
        if (need_calculated_) {
            result_ = task_->getResult();
            need_calculated_ = false;
        }
    }

    template <typename r_t>
    r_t getResult() {
        caclulate();
        
        return result_.Get<r_t>();
    }
};

template <typename r_t>
class FutureResult {
    private:
    std::shared_ptr<Task> task_;
    
    public:
    ~FutureResult() = default;

    FutureResult() = default;

    FutureResult(const Task& new_task) {
        task_ = std::make_shared<Task>(new_task);
    }

    template <typename T>
    FutureResult(const T& new_task) {
        task_ = std::make_shared<Task>(new_task);
    }

    template <typename T>
    FutureResult(const std::shared_ptr<T>& new_task) {
        task_ = new_task;
    }

    FutureResult(const FutureResult<r_t>& copy) {
        task_ = copy.task_;
    }

    FutureResult operator=(const FutureResult<r_t>& other) {
        task_ = other.task_;
        return *this;
    }   

    r_t getResult() {
        return task_->getResult<r_t>();
    }
};

class Scheduler {
    private:
    std::vector<std::shared_ptr<Task>> tasks_;

    template <typename r_t>
    FutureResult<r_t> makeFutureResult(const FutureResult<r_t>& future_result) {
        return future_result;
    }

    template <typename r_t>
    FutureResult<r_t> makeFutureResult(const r_t& value) {
        return FutureResult<r_t>(value);
    }

    public:
    template <typename T, typename t1, typename t2>
    std::shared_ptr<Task> add(const T& task_func, const t1& task_1, const t2& task_2) {
        std::shared_ptr<Task> new_task = std::make_shared<Task>(Task(task_func, makeFutureResult(task_1), makeFutureResult(task_2)));

        tasks_.push_back(new_task);
        return new_task;
    }

    template <typename T, typename t1>
    std::shared_ptr<Task> add(const T& task_func, const t1& task_1) {
        std::shared_ptr<Task> new_task = std::make_shared<Task>(Task(task_func, makeFutureResult(task_1)));

        tasks_.push_back(new_task);
        return new_task;
    }

    template <typename r_t, typename t>
    FutureResult<r_t> getFutureResult(const std::shared_ptr<t>& task) {
        return  FutureResult<r_t>(task);
    }

    template <typename r_t>
    r_t getResult(const std::shared_ptr<Task>& task) {
        return task->getResult<r_t>();
    }

    void executeAll() {
        for (auto task : tasks_) {
            task->caclulate();
        }
    }
};
