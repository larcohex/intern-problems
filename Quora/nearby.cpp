#include <iostream>
#include <cmath>
#include <vector>


using namespace std;


struct Topic
{
	unsigned int id;
	double x, y;
	double dist;
};


struct Question
{
	unsigned int id;
	unsigned int nOfTopics;
	vector <unsigned int> topics;
	int relevance;
};


struct Query
{
	char type;
	unsigned int nOfResults;
	double x, y;
};


void initDist (vector <Topic> &top, Query &q)
{
	for (unsigned int i = 0; i < top.size(); ++i)
	{
		(top[i]).dist = sqrt ( pow (((top[i]).x - q.x), 2) + pow (((top[i]).y - q.y), 2) );
	}
}


void mergeTopics (vector <Topic> &top, unsigned int begin, unsigned int end)
{
	unsigned int middle = (begin + end) / 2;
	unsigned int i, j;
	vector <Topic> sortedTop;
	for (i = begin, j = middle + 1; (i <= middle) && (j <= end);)
	{
		if ((top[i]).dist < (top[j]).dist)
		{
			sortedTop.push_back (top[i]);
			++i;
		}
		else
		{
			sortedTop.push_back (top[j]);
			++j;
		}
	}
	while (i <= middle)
	{
		sortedTop.push_back (top[i]);
		++i;
	}
	while (j <= end)
	{
		sortedTop.push_back (top[j]);
		++j;
	}
	for (i = begin; i <= end; ++i)
	{
		top[i] = sortedTop[i - begin];
	}
}


void mergeSortTopics (vector <Topic> &top, unsigned int begin, unsigned int end)
{
	if (end > begin)
	{
		unsigned int middle = (begin + end) / 2;
		mergeSortTopics (top, begin, middle);
		mergeSortTopics (top, middle + 1, end);
		mergeTopics (top, begin, end);
	}
}


void processTopics (vector <Topic> &top, unsigned int n)
{
	if (n > 0)
	{
		cout << (top[0]).id;
		for (unsigned int i = 1; i < n; ++i)
		{
			cout << " " << (top[i]).id;
		}
	}
	cout << endl;
}


void initQTopics (vector <Question> &quest, vector <Topic> &top)
{
	for (unsigned int i = 0; i < quest.size(); ++i)
	{
		if (quest[i].nOfTopics == 0)
		{
			quest.erase (quest.begin() + i);
			--i;
		}
		else
		{
			for (unsigned int j = 0; j < top.size(); ++j)
			{
				bool temp = false;
				for (unsigned int k = 0; k < (quest[i]).nOfTopics; ++k)
				{
					if ((quest[i]).topics[k] == (top[j]).id)
					{
						quest[i].relevance = j;
						temp = true;
						break;
					}
				}
				if (temp)
				{
					break;
				}
			}
		}
	}
}


void mergeQuestions (vector <Question> &quest, unsigned int begin, unsigned int end)
{
	unsigned int middle = (begin + end) / 2;
	unsigned int i, j;
	vector <Question> sortedQuest;
	for (i = begin, j = middle + 1; (i <= middle) && (j <= end);)
	{
		if ((quest[i]).relevance < (quest[j]).relevance)
		{
			sortedQuest.push_back (quest[i]);
			++i;
		}
		else if ((quest[i]).relevance > (quest[j]).relevance)
		{
			sortedQuest.push_back (quest[j]);
			++j;
		}
		else
		{
			if ((quest[i]).id > (quest[j].id))
			{
				sortedQuest.push_back (quest[i]);
				++i;
			}
			else
			{
				sortedQuest.push_back (quest[j]);
				++j;
			}
		}
	}
	while (i <= middle)
	{
		sortedQuest.push_back (quest[i]);
		++i;
	}
	while (j <= end)
	{
		sortedQuest.push_back (quest[j]);
		++j;
	}
	for (i = begin; i <= end; ++i)
	{
		quest[i] = sortedQuest[i - begin];
	}
}


void mergeSortQuestions (vector <Question> &quest, unsigned int begin, unsigned int end)
{
	if (end > begin)
	{
		unsigned int middle = (begin + end) / 2;
		mergeSortQuestions (quest, begin, middle);
		mergeSortQuestions (quest, middle + 1, end);
		mergeQuestions (quest, begin, end);
	}
}


void processQuestions (vector <Question> &quest, unsigned int n)
{
	if (n > 0)
	{
		cout << quest[0].id;
		for (unsigned int i = 1; i < n && i < quest.size(); ++i)
		{
			if (quest[i].relevance != -1)
			{
				cout << " " << quest[i].id;
			}
		}
	}
	cout << endl;
}


int main()
{
	unsigned int i, j;
	unsigned int T, Q, N;
	cin >> T;
	cin.get();
	cin >> Q;
	cin.get();
	cin >> N;
	cin.get();
	vector <Topic> top;
	vector <Question> quest;
	vector <Query> queries;
	for (i = 0; i < T; ++i)
	{
		unsigned int id;
		double x, y;
		cin >> id;
		cin.get();
		cin >> x;
		cin.get();
		cin >> y;
		cin.get();
		Topic topic;
		topic.id = id;
		topic.x = x;
		topic.y = y;
		top.push_back (topic);
	}
	for (i = 0; i < Q; ++i)
	{
		unsigned int id;
		unsigned int nOfTopics;
		vector <unsigned int> topics;
		cin >> id;
		cin.get();
		cin >> nOfTopics;
		cin.get();
		for (j = 0; j < nOfTopics; ++j)
		{
			unsigned int topic;
			cin >> topic;
			cin.get();
			topics.push_back (topic);
		}
		Question question;
		question.id = id;
		question.nOfTopics = nOfTopics;
		question.topics = topics;
		quest.push_back (question);
	}
	for (i = 0; i < N; ++i)
	{
		char type;
		unsigned int nOfResults;
		double x, y;
		cin >> type;
		cin.get();
		cin >> nOfResults;
		cin.get();
		cin >> x;
		cin.get();
		cin >> y;
		cin.get();
		Query query;
		query.type = type;
		query.nOfResults = nOfResults;
		query.x = x;
		query.y = y;
		queries.push_back (query);
	}
	for (i = 0; i < N; ++i)
	{
		initDist (top, queries[i]);
		mergeSortTopics (top, 0, T - 1);
		if (queries[i].type == 't')
		{
			processTopics (top, (queries[i]).nOfResults);
		}
		else
		{
			initQTopics (quest, top);
			mergeSortQuestions (quest, 0, Q - 1);
			processQuestions (quest, (queries[i]).nOfResults);
		}
	}
}
