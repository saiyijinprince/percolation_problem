#include <utility>
#include <vector>
#include <boost\shared_ptr.hpp>

using namespace std;

/*	Given a 2-d array of 0s and 1s, find a path from top to bottom.
	You can only move on 1s and you can only move right and down, only
	only if there's a one.
*/

namespace {
	const int n = 5;
	const int m = 5;
}

class QuickFind {
public:
	QuickFind(int (*arr)[n][m], int rows, int cols);
	bool hasPathToRoot(int row, int col, int& sizeOfPath);

	vector<pair<int, int>> getPath();
	bool walkUpPath(boost::shared_ptr<pair<int, int>> p, vector<pair<int, int>>& recordedPath);
private:
	vector<vector<boost::shared_ptr<pair<int, int>>>> m_ids;
};

QuickFind::QuickFind(int (*arr)[n][m], int rows, int cols)
{
	m_ids.resize(rows);
	int curr_root = 0;
	for (int i=0; i<rows; ++i) {
		m_ids[i].resize(cols);

		for (int j=0; j<cols; ++j) {
			int val = (*arr)[i][j];
			if ( val == 0) {
				continue;
			}

			int left = j - 1;
			int top = i - 1;

			//top left corner
			if (left < 0 && top < 0) {
				m_ids[i][j].reset(new pair<int, int>(i, j));
				continue;
			}

			//If you're on the top row, you're always at the root.
			if (top < 0){
				m_ids[i][j].reset(new pair<int, int>(i, j));
				continue;			
			}

			//First column - there's nothing to the left so its parent can
			//only be itself or the one above it.  Always chose the one above it
			//first
			if (left < 0) {
				//Check the top
				int sizeOfPath = 0;
				if (m_ids[top][j] && hasPathToRoot(top, j, sizeOfPath)){
					m_ids[i][j].reset(new pair<int, int>(top, j));
				}

				continue;
			}

			
			//if there's a one on the top and to the left.
			//chose which one can lead to the top row.
			//If both can reach the top, chose the shortest path.
			int leftSizeOfPath = 0, topSizeOfPath = 0;
			bool bTop = hasPathToRoot(top, j, topSizeOfPath);
			bool bLeft	= hasPathToRoot(i, left, leftSizeOfPath);
			
			if (bLeft && bTop) {
				if (leftSizeOfPath < topSizeOfPath) {
					m_ids[i][j].reset(new pair<int, int>(i,left));
				} else {
					m_ids[i][j].reset(new pair<int, int>(top, j));
				}
			} else if (bLeft) {
				m_ids[i][j].reset(new pair<int, int>(i,left));
			} else if (bTop) {
				m_ids[i][j].reset(new pair<int, int>(top, j));
			}
		}
	}
}

bool QuickFind::hasPathToRoot(int row, int col, int& sizeOfPath)
{
	boost::shared_ptr<pair<int, int>> p = m_ids[row][col];
	if (!p)
		return false;
	
	//Set to 1 for the current p
	sizeOfPath = 1;

	while (p && p->first !=0) {
		sizeOfPath++;		
		p = m_ids[p->first][p->second];
	}

	if (!p || p->first !=0){
		sizeOfPath = 0;
		return false;
	}

	return true;
}

bool QuickFind::walkUpPath(boost::shared_ptr<pair<int, int>> p, vector<pair<int, int>>& recordedPath)
{
	boost::shared_ptr<pair<int, int>> temp = p;
	if (!temp)
		return false;

	while (temp && temp->first != 0) {
		recordedPath.push_back(*temp);
		temp = m_ids[temp->first][temp->second];
	}

	if (!temp || temp->first != 0) 
		return false;

	recordedPath.push_back(*temp);
	return true;
}

vector<pair<int, int>> QuickFind::getPath()
{
	vector<pair<int, int>> path;
	//start from the last row and move upwards to see if 
	//there's a path to the first row.
	int last_row = m_ids.size()-1;
	for (int i=0; i<m_ids[i].size(); ++i){
		boost::shared_ptr<pair<int, int>> p = m_ids[last_row][i];
		if (!p)
			continue;

		if (walkUpPath(p, path)){
			path.insert(path.begin(), pair<int,int>(last_row, i));
			return path;
		}
	}
	return path;
}

int main() 
{
	int arr[n][m] = 
	{
		{1,0,0,0,0},
		{1,0,0,1,0},
		{1,1,1,0,0},
		{0,1,1,0,0},
		{0,0,1,0,0}
	};

	QuickFind qf(&arr, n, m);
	vector<pair<int, int>> path = qf.getPath();

	return 0;
}