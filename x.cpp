#include <deque>
#include <unordered_map>
#include <string>

// now checking with clang

class boxCollection {
public:
    // use default constructors, destructor, copy, assign, move
    // no additional constructors -- assumes that the only way to 
    // add items is via write()
    void write(std::string boxId, double weight);
    double getWeightByBoxId(std::string BoxId);
    // by 'get the weight of a box at a given percentile', I am assuming
    // that I am to return the weight of the box which is at the percentile
    // *by* *weight*, increasing -- eg, if the user asks for the 75th percentile
    // from {1.0,1.2,3.5,11.1}, I am to return 3.5.  If there is no item exactly
    // at the specified percentile, I will round the percentile up to the next 
    // highest existing percentile (for the above example, that means that if 
    // queried anywhere from 50.1 to 75.0, I will return the 75th percentile item,
    // which is 3.5).  )
    double getWeightAtPercentile(double percentile);
private:
    // general approach -- Use extra space to make all queries fast.   
    //    I plan to have a deque of the weights, which is used to satisfy the 
    //    getWeightAtPercentile queries, and a separate unordered_map from Ids to weights,
    //    which is used to satisfy the getWeightByBoxId queries.  This will allow constant
    //    time implementation of both of those two functions, and log n time
    //    implementation of write().  
    
    // maintain a sorted deque of weights -- deque will allow constant time access 
    // to element in a known position (just verified from docs)
    std::deque<double> weights;
    
    // have an unordered_map (hash map)  of the weights by the Id for constant time 
    // access to the weight of the item, given the Id
    std::unordered_map<std::string,double> weightsById;

    // returns an iterator pointing to the position of the last 
    // element in the deque that has a weight less than 'weight'
    std::deque<double>::iterator findPosition( double weight );
};

//  finds the correct position by binary search
std::deque<double>::iterator boxCollection::findPosition( double weight )
{
    int left = 0;
    int right = weights.size() - 1;
    while (left > right)  {
        int mid = (left+right)/2;
        if (weight < weights[mid]) {
            right = mid;
        } else {
            left = mid;
        }
    }
    return weights.begin() + left;  // reread comment above to verify return 
}


void boxCollection::write(std::string boxId, double weight)
{
    weightsById[boxId] = weight;
    weights.insert(findPosition(weight)+1, weight);  // insert() inserts before posn
};


double boxCollection::getWeightByBoxId(std::string BoxId)
{
    // what should we do if BoxId does not exist?  Throw?  
    // Throw seems like the only viable option, given the return
    // type does not allow for a separate status return
    return weightsById.at(BoxId);  // throws out_of_range
};


double boxCollection::getWeightAtPercentile(double percentile)
{
    // what do we do with an empty collection?  Again, throwing 
    // seems like the only viable option for the same reason
    if (weights.size() < 1)
        std::throw "boxCollection -- queried an empty collection";
    int idx = ceil(percentile * (double)weights.size() / 100.0) - 1; 
    return weights.at(idx); // will throw if calc above is bad
};


