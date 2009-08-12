#include "graph.hpp"
//////////////////////////////////////////////////////////////////////////
struct pushbackAndRemoveNode
{
    pushbackAndRemoveNode( std::vector<std::string> &aNV, Graph* const &aG ):aNodeVec(aNV),G(aG){};
    void operator()( const std::string& aNode )
    {
        aNodeVec.push_back(aNode);
        G->removeNode(aNode);
    }
    std::vector<std::string>& aNodeVec;
    Graph* const &G;
};
//////////////////////////////////////////////////////////////////////////
struct appendUniqueNodeToNodeVecFunctor
{
    appendUniqueNodeToNodeVecFunctor(std::vector<std::string>& aNodeVec):mNodeVec(aNodeVec){};
    void operator()( const std::string& aNode ) const
    {
        appendUniqueNodeToNodeVec( aNode, mNodeVec );
    }
    std::vector<std::string>& mNodeVec;
};
//////////////////////////////////////////////////////////////////////////
struct pairFirstEqualsNumber
{
    pairFirstEqualsNumber(unsigned ui): i(ui) {}
    bool operator()( const std::pair< unsigned, boost::shared_ptr<GraphNode> >& aPair ) const
    {
        return ((aPair.first) == i);
    }
    unsigned i;
};
//////////////////////////////////////////////////////////////////////////
struct pushbackPairSecondName
{
    std::string operator() ( const std::pair< unsigned, boost::shared_ptr<GraphNode> >& aGNPUI ) const
    {
        return ((aGNPUI.second)->getName());
    }
};
//////////////////////////////////////////////////////////////////////////
struct compareGraphNodePtrToNode
{
    compareGraphNodePtrToNode( std::string aN ): n(aN) {};
    bool operator() ( const boost::shared_ptr<GraphNode>& pgn) const
    {
        return ( pgn->getName() == n );
    }
    std::string n;
};
//////////////////////////////////////////////////////////////////////////
struct isRootNodePtr
{
    bool operator()( const boost::shared_ptr<GraphNode>& aGNP ) const
    {
        return !(aGNP->hasParents()) ;
    }
};
//////////////////////////////////////////////////////////////////////////
struct isLeafNodePtr
{
    bool operator()( const boost::shared_ptr<GraphNode>& aGNP ) const
    {
        return !(aGNP->hasChildren()) ;
    }
};
//////////////////////////////////////////////////////////////////////////
struct setDependencyFunctor
{
    setDependencyFunctor( Graph& aG ) : aGraph(aG){};
    void operator()( const std::pair<std::string, std::string>& aNodePair ) const
    {
        aGraph.setDependency( aNodePair.first, aNodePair.second );
    }
    Graph& aGraph;
};
//////////////////////////////////////////////////////////////////////////
struct addNodeToGraph
{
    addNodeToGraph( Graph& aG ) : aGraph(aG){};
    void operator()( const std::string& aNode ) const
    {
        aGraph.addNode( aNode );
    }
    Graph& aGraph;
};
//////////////////////////////////////////////////////////////////////////
template<typename T>
struct pushbackGraphNodeNames
{
    std::string operator() ( const T& aGNP ) const
    {
        return (aGNP->getName());
    }
};
//////////////////////////////////////////////////////////////////////////
struct pairSecondEqualsNode
{
    pairSecondEqualsNode( const std::string& N ):n(N){};
    bool operator()(const std::pair< unsigned, boost::shared_ptr<GraphNode> >& p) const 
    {
        return ((p.second)->getName() == n);
    }
    std::string n;
};
//////////////////////////////////////////////////////////////////////////
struct deleteObjectFunctor
{
    template<typename T>
    void operator()(T*& ptr) const
    {
        delete ptr;
        ptr = 0;
    }
};
//////////////////////////////////////////////////////////////////////////
struct setTopSortData
{
    setTopSortData( unsigned& dist, const Graph* const &aG,
        std::vector<std::string>& chilCollVec, std::vector<std::string>& tNodeVec,
        std::pair< unsigned, boost::shared_ptr<GraphNode> >& d_gnp_pair,
        std::vector< std::pair< unsigned, boost::shared_ptr<GraphNode> > >& tPVec,
        std::vector< std::pair< unsigned, boost::shared_ptr<GraphNode> > >& d_gnp_p_vec ) 
        :   distance(dist), thisGraph(aG), childrenCollecterVec(chilCollVec),
        tempNodesVec(tNodeVec), depth_graphnodeptr_pair(d_gnp_pair),
        tempPairVec(tPVec), depth_graphNodePtr_pair_vec(d_gnp_p_vec) 
    { }
    void operator()( const std::string& aNode )
    {
        std::vector<std::pair<unsigned, boost::shared_ptr<GraphNode> > >::iterator 
            depth_graphNodePtr_pair_vec_iter = 
            getNodeIterInPairVec( aNode, depth_graphNodePtr_pair_vec );
        if ( depth_graphNodePtr_pair_vec_iter != depth_graphNodePtr_pair_vec.end() )   //If this node has already been seen
            (*depth_graphNodePtr_pair_vec_iter).first = distance;                      //update its distance.
        else
        {
            depth_graphnodeptr_pair.first = distance;                                  //else add new element to the current
            depth_graphnodeptr_pair.second = *(thisGraph->getNodeIterInGraph(aNode)) ;     //depth and boost::shared_ptr<GraphNode> pair vector.
            depth_graphNodePtr_pair_vec.push_back( depth_graphnodeptr_pair );          //with present depth.
        }
        thisGraph->getChildrenOf( aNode, tempNodesVec );  //Add the children of this GraphNode into a temporary children holder.
        for_all( tempNodesVec, appendUniqueNodeToNodeVecFunctor(childrenCollecterVec) ); //Add the children of node without repetition
    }
    unsigned& distance;
    const Graph* const &thisGraph;
    std::vector<std::string>& childrenCollecterVec;
    std::vector<std::string>& tempNodesVec;
    std::pair< unsigned, boost::shared_ptr<GraphNode> >& depth_graphnodeptr_pair;
    std::vector< std::pair< unsigned, boost::shared_ptr<GraphNode> > >& tempPairVec;
    std::vector< std::pair< unsigned, boost::shared_ptr<GraphNode> > >& depth_graphNodePtr_pair_vec;
};
//////////////////////////////////////////////////////////////////////////
std::vector< std::pair<unsigned, boost::shared_ptr<GraphNode>> >::iterator 
getNodeIterInPairVec ( const std::string& aNode, 
                        std::vector< std::pair<unsigned, boost::shared_ptr<GraphNode>> >& aPairVec )
{
    return find_if( aPairVec.begin(), aPairVec.end(), pairSecondEqualsNode(aNode) );
}
//////////////////////////////////////////////////////////////////////////
std::vector<std::string>::const_iterator getNodeIterInNodeVec( const std::string& aNode, const std::vector<std::string>& aNodeVec )  
{	
    return find(aNodeVec.begin(), aNodeVec.end(), aNode);
}
//////////////////////////////////////////////////////////////////////////
void appendUniqueNodeToNodeVec(const std::string& aNode, std::vector<std::string>& aNodeVec)
{
    std::vector<std::string>::const_iterator iter = getNodeIterInNodeVec( aNode, aNodeVec );
    if ( iter == aNodeVec.end() )
    {
        aNodeVec.push_back( aNode );
    }
}
//////////////////////////////////////////////////////////////////////////
void Graph::addNode( const std::string& aNode )
{
    if (!doesNodeExist(aNode))
    {
        mGraphNodes.push_back( boost::shared_ptr<GraphNode>( new GraphNode(aNode) ) );
#ifdef ENABLE_TOPOLOGICALSORT_DEBUGPRINTS
        std::cout << "Adding node: " << aNode << std::endl;
#endif
    }
    else
    {
#ifdef ENABLE_TOPOLOGICALSORT_DEBUGPRINTS
        std::cout << "Can not add node : " << aNode << " already exists" << std::endl;
#endif
    }
}
//////////////////////////////////////////////////////////////////////////
void addNodes( Graph& G, const std::vector<std::string>& aNodeVector )
{
    for_all( aNodeVector, addNodeToGraph(G) );
}
//////////////////////////////////////////////////////////////////////////
void Graph::removeNode( const std::string& aNode )
{
    std::vector< boost::shared_ptr<GraphNode> >::iterator iter = find_if( mGraphNodes.begin(), mGraphNodes.end(), compareGraphNodePtrToNode(aNode) );

    if ( iter != mGraphNodes.end() )
    {
        mGraphNodes.erase(iter);
#ifdef ENABLE_TOPOLOGICALSORT_DEBUGPRINTS
        std::cout << "std::string: " << aNode << " removed" << std::endl;
#endif
    } 
    else
    {
#ifdef ENABLE_TOPOLOGICALSORT_DEBUGPRINTS
        std::cout << "std::string : " << aNode << " does not exist" << std::endl;
#endif
    }
}
//////////////////////////////////////////////////////////////////////////
unsigned Graph::getNumberOfNodes( void ) const 
{ 
    return static_cast<unsigned>( mGraphNodes.size() ); 
}
//////////////////////////////////////////////////////////////////////////
void Graph::setDependency( const std::string& aParent, const std::string& aChild )
{
    std::vector< boost::shared_ptr<GraphNode> >::const_iterator pI = getNodeIterInGraph(aParent);
    std::vector< boost::shared_ptr<GraphNode> >::const_iterator cI = getNodeIterInGraph(aChild);

    if( pI != mGraphNodes.end() )       //The reason for this nesting 
        if ( cI != mGraphNodes.end() )  //of ifs is to discern exactly
        {                               //if parent is missing or child
            if (!((*pI)->doesChildExist(aChild))) //Check if dependency already
            {                                     //exists
                (*pI)->addChild( (*cI) ); //Also sets pI as Parent of cI
#ifdef ENABLE_TOPOLOGICALSORT_DEBUGPRINTS
                std::cout << "Setting dependency : " << aParent << "->"
                    << aChild << std::endl;
#endif
            } 
            else
            {
#ifdef ENABLE_TOPOLOGICALSORT_DEBUGPRINTS
                std::cout << "Dependency : " << aParent << "->"
                    << aChild << " already exists" << std::endl;
#endif
            }
        }
        else
        {
#ifdef ENABLE_TOPOLOGICALSORT_DEBUGPRINTS
            std::cout << "Child: " << aChild << " does not exist" << std::endl;
#endif
        }
    else
    {
#ifdef ENABLE_TOPOLOGICALSORT_DEBUGPRINTS
        std::cout <<"Parent: " << aParent << " does not exist" << std::endl;
#endif
    }
}
//////////////////////////////////////////////////////////////////////////
void setDependencies ( Graph& G, const std::vector< std::pair< std::string, std::string> >& aNodePairVec )
{
    for_all( aNodePairVec, setDependencyFunctor(G));
}
//////////////////////////////////////////////////////////////////////////
void Graph::getLeafNodes( std::vector<std::string>& aNodeVec ) const
{
    aNodeVec.clear();
    std::vector< boost::shared_ptr<GraphNode> > temp;

    //Copy all leaf node pointers to temp
    copy_if( mGraphNodes.begin(), mGraphNodes.end(), std::back_inserter(temp), 
             isLeafNodePtr() ); 

    //Convert leaf node pointer to their corresponding names and append to
    //the input argument vector (aNodeVec).
    transform( temp.begin(), temp.end(), std::back_inserter(aNodeVec), 
               pushbackGraphNodeNames<boost::shared_ptr<GraphNode>>() ); //
}
//////////////////////////////////////////////////////////////////////////
void Graph::getRootNodes( std::vector<std::string>& aNodeVec ) const
{
    aNodeVec.clear();
    std::vector< boost::shared_ptr<GraphNode> > temp;

    //Copy all root node pointers to temp
    copy_if( mGraphNodes.begin(), mGraphNodes.end(), std::back_inserter(temp), 
             isRootNodePtr() );

    //Convert root node pointer to their corresponding names and append to
    //the input argument vector (aNodeVec).
    transform( temp.begin(), temp.end(), std::back_inserter(aNodeVec), 
               pushbackGraphNodeNames<boost::shared_ptr<GraphNode>>() );
}
//////////////////////////////////////////////////////////////////////////
bool Graph::isRootNode( const std::string& aNode ) const
{ 
	std::vector< boost::shared_ptr<GraphNode> >::const_iterator iter = getNodeIterInGraph(aNode);
	if( iter != mGraphNodes.end() )
		return (!( (*iter)->hasParents() ));
	else
	{
#ifdef ENABLE_TOPOLOGICALSORT_DEBUGPRINTS
		std::cout << "std::string : " << aNode << " does not exist" << std::endl;
#endif
		return false;
	}
}
//////////////////////////////////////////////////////////////////////////
bool Graph::isLeafNode( const std::string& aNode ) const
{ 
	std::vector< boost::shared_ptr<GraphNode> >::const_iterator iter = getNodeIterInGraph(aNode);
	if( iter != mGraphNodes.end() )
		return (!( (*iter)->hasChildren() ));
	else
	{
#ifdef ENABLE_TOPOLOGICALSORT_DEBUGPRINTS
		std::cout << "std::string : " << aNode << " does not exist" << std::endl;
#endif
		return false;
	}
}
//////////////////////////////////////////////////////////////////////////
std::vector< boost::shared_ptr<GraphNode> >::const_iterator Graph::getNodeIterInGraph( const std::string& aNode ) const
{
    return find_if( mGraphNodes.begin(), mGraphNodes.end(), compareGraphNodePtrToNode(aNode) );
}
//////////////////////////////////////////////////////////////////////////
void Graph::getParentsOf( const std::string& aNode, std::vector<std::string>& aNodeVec ) const
{
    std::vector< boost::shared_ptr<GraphNode> >::const_iterator iter = getNodeIterInGraph(aNode);
    aNodeVec.clear();  
    if( iter != mGraphNodes.end() )
        if( (*iter)->hasParents() )
        {
            std::vector<GraphNode*> gNPV = ((*iter)->getParentVec());

            // Append the names of the Parent graph nodes to aNodeVec
            transform(gNPV.begin(), gNPV.end(),  std::back_inserter(aNodeVec),
                      pushbackGraphNodeNames<GraphNode*>());
        }
        else
        {
#ifdef ENABLE_TOPOLOGICALSORT_DEBUGPRINTS
            std::cout << "std::string : " << aNode << " has no Parents" << std::endl;
#endif
        }
    else
    {
#ifdef ENABLE_TOPOLOGICALSORT_DEBUGPRINTS
        std::cout << "std::string : " << aNode << " does not exist" << std::endl;
#endif
    }
}
//////////////////////////////////////////////////////////////////////////
void Graph::getChildrenOf( const std::string& aNode, std::vector<std::string>& aNodeVec ) const
{
    std::vector< boost::shared_ptr<GraphNode> >::const_iterator iter = getNodeIterInGraph(aNode);
    aNodeVec.clear();
    if( iter != mGraphNodes.end() )
        if( (*iter)->hasChildren() )
        {
            std::vector< boost::shared_ptr<GraphNode> > gNPV = ((*iter)->getChildrenVec());

            // Append the names of the Children graph nodes to aNodeVec
            transform(gNPV.begin(), gNPV.end(),  std::back_inserter(aNodeVec),
                      pushbackGraphNodeNames<boost::shared_ptr<GraphNode>>());
        }
        else
        {
#ifdef ENABLE_TOPOLOGICALSORT_DEBUGPRINTS
            std::cout << "std::string : " << aNode << " has no Children" << std::endl;
#endif
        }
    else
    {
#ifdef ENABLE_TOPOLOGICALSORT_DEBUGPRINTS
        std::cout << "std::string : " << aNode << " does not exist" << std::endl;
#endif
    }
}
//////////////////////////////////////////////////////////////////////////
void Graph::topologicalSortRoot( std::vector<std::string>& aNodeVec )
{
    std::vector<std::string> tempRootNodeVec ; 
    this->getRootNodes( tempRootNodeVec ) ;
    unsigned nodesCounter = getNumberOfNodes() ;
    unsigned totalNodes = getNumberOfNodes() ;
    if ( nodesCounter )
    {
        aNodeVec.clear() ;
        do 
        {
            for_all( tempRootNodeVec, pushbackAndRemoveNode(aNodeVec, this) );
            --nodesCounter;
            this->getRootNodes( tempRootNodeVec );
        } 
        while ( this->getNumberOfNodes() &&  nodesCounter ); //If more nodes, and the iteration has taken place 
                                                             //numOfNode times (if more then the graph would be cyclic)
        if ( aNodeVec.size() != totalNodes)
        {
            std::cout << "The Graph was not acyclic : Cycle of " 
                      << totalNodes - aNodeVec.size() << " nodes." 
                      << std::endl;
        }
    }                                                        
    else
    {
        std::cout << "Can not topologically sort the graph, no nodes" << std::endl;
    }
}
//////////////////////////////////////////////////////////////////////////
void Graph::topologicalSort ( std::vector<std::string>& aNodeVec ) const
{
    aNodeVec.clear();
    std::pair< unsigned, boost::shared_ptr<GraphNode> > depth_graphnodeptr_pair;
    std::vector< std::pair< unsigned, boost::shared_ptr<GraphNode> > > depth_graphNodePtr_pair_vec; 
    std::vector< std::pair< unsigned, boost::shared_ptr<GraphNode> > > tempPairVec;
    std::vector<std::string> currentNodesVec;
    std::vector<std::string> tempNodesVec;
    std::vector<std::string> childrenCollecterVec;
    getRootNodes( currentNodesVec ); //Starting from all root nodes in the current nodes vector.
    unsigned distance(0); //Set starting distance to 0.
    unsigned maxIterationCounter = getNumberOfNodes(); //If any node has depth greater than the 
                                                       // total number of nodes, then the graph
                                                       // is cyclic. Hence stop finding the sort order.

    while ( !( currentNodesVec.empty() ) && (maxIterationCounter) ) //Either there will be no children left i.e
    {                                                               //all currentNodes are leaf, so we finish or there are cycles.
        for_all( currentNodesVec, setTopSortData (distance, this, childrenCollecterVec, 
                                                  tempNodesVec, depth_graphnodeptr_pair,
                                                  tempPairVec, depth_graphNodePtr_pair_vec ) );
        currentNodesVec.clear();
        currentNodesVec = childrenCollecterVec; // Now move down a depth level, so children become current nodes.
        childrenCollecterVec.clear();
        tempNodesVec.clear();
        ++ distance;
        --maxIterationCounter;
    }
    for ( unsigned i = 0; i != getNumberOfNodes(); ++i ) //Add the pairs with the correct depth pairs in ascending order to a temporary pair vector
        copy_if( depth_graphNodePtr_pair_vec.begin(), depth_graphNodePtr_pair_vec.end(), std::back_inserter(tempPairVec), 
                 pairFirstEqualsNumber(i) );

    transform( tempPairVec.begin(), tempPairVec.end(),  //Add the names from the temporary pair vector to the input argument node vector (aNodeVector)
               std::back_inserter(aNodeVec), pushbackPairSecondName());
}
//////////////////////////////////////////////////////////////////////////
bool Graph::doesNodeExist( const std::string& aNode ) const
{
    std::vector< boost::shared_ptr<GraphNode> >::const_iterator iter = getNodeIterInGraph(aNode);
    if( iter != mGraphNodes.end() )
    {
#ifdef ENABLE_TOPOLOGICALSORT_DEBUGPRINTS
        std::cout << "std::string : " << aNode << " exists" << std::endl;
#endif
        return true;
    }
    else
    {
#ifdef ENABLE_TOPOLOGICALSORT_DEBUGPRINTS
        std::cout << "std::string : " << aNode << " does not exist" << std::endl;
#endif
        return false;
    }
}
//////////////////////////////////////////////////////////////////////////
void appendNodeFrom( const std::string& aNode, std::vector<std::string>& aNodeVec ) 
{
    std::vector<std::string>::const_iterator i = getNodeIterInNodeVec( aNode, aNodeVec );
    if ( i == aNodeVec.end() )
    {
        aNodeVec.push_back( aNode );
#ifdef ENABLE_TOPOLOGICALSORT_DEBUGPRINTS
        std::cout << "std::string : " << aNode << " appended" << std::endl;
#endif
    } 
    else
    {
#ifdef ENABLE_TOPOLOGICALSORT_DEBUGPRINTS
        std::cout << "std::string " << aNode << " can not be appended again" << std::endl;
#endif
    }
}
//////////////////////////////////////////////////////////////////////////
void removeNodeFrom( const std::string& aNode, std::vector<std::string>& aNodeVec ) 
{
    std::vector<std::string>::iterator i = std::find( aNodeVec.begin(), aNodeVec.end(), aNode );
    if ( i == aNodeVec.end() )
    {
#ifdef ENABLE_TOPOLOGICALSORT_DEBUGPRINTS
        std::cout << "std::string " << aNode << " does not exist" << std::endl;
#endif
    }
    else
    {
        aNodeVec.erase(i);
#ifdef ENABLE_TOPOLOGICALSORT_DEBUGPRINTS
        std::cout << "std::string : " << aNode << " removed" << std::endl;
#endif
    }
}
//////////////////////////////////////////////////////////////////////////