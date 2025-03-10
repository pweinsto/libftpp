#ifndef OBSERVER_HPP
# define OBSERVER_HPP

# include <functional>
# include <unordered_map>
# include <vector>

template<typename TEvent>
class Observer
{
public:
	void subscribe(const TEvent& event, const std::function<void()>& lambda)
	{
		subscribers[event].push_back(lambda);
	}

	void notify(const TEvent& event)
	{
		
		if (subscribers.find(event) != subscribers.end())
		{
			for (const std::function<void()>& lambda : subscribers[event])
			{
				lambda();
			}
		}
	}

private:
	std::unordered_map<TEvent, std::vector<std::function<void()>>> subscribers;
};

# endif // OBSERVER_HPP