#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <functional>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <type_traits>
#include <iterator>
#include <optional>


// 枚举对象类型

// 邻接表中储存每个结点的邻居结点的数据类型
enum class Container {VEC, LIST, SET, UNORDERED_SET, MULTISET, UNORDERED_MULTISET};

// 可自环
enum class SelfLoop { ALLOWED, DISALLOWED};

// 可重复边
enum class MultiEdge { ALLOWED, DISALLOWED};

// 有向边
enum class EdgeDirection {DIRECTED, UNDIRECTED};

// 邻接表外层的数据类型
enum class Map {MAP, UNORDERED_MAP};




// 根据 Container 枚举选择邻居容器类型

template<Container C, typename N>
struct NeighborContainerSelector;

template<typename N> // 邻居用 unordered_set<N>
struct NeighborContainerSelector<Container::UNORDERED_SET, N> {
    using type = std::unordered_set<N>;
};

template<typename N> // 邻居用 set<N>
struct NeighborContainerSelector<Container::SET, N> {
    using type = std::set<N>;
};

template<typename N> // 邻居用 multiset<N>
struct NeighborContainerSelector<Container::MULTISET, N> {
    using type = std::multiset<N>;
};

template<typename N> // 邻居用 unordered_multiset<N>
struct NeighborContainerSelector<Container::UNORDERED_MULTISET, N> {
    using type = std::unordered_multiset<N>;
};

template<typename N> // 邻居用 vector<N>
struct NeighborContainerSelector<Container::VEC, N> {
    using type = std::vector<N>;
};

template<typename N> // 邻居用 list<N>
struct NeighborContainerSelector<Container::LIST, N> {
    using type = std::list<N>;
};




// 根据 Map 枚举选择外层容器类型

template<Map M, typename Key, typename Value>
struct AdjListSelector;

template<typename Key, typename Value>
struct AdjListSelector<Map::UNORDERED_MAP, Key, Value> {
    using type = std::unordered_map<Key, Value>;
};

template<typename Key, typename Value>
struct AdjListSelector<Map::MAP, Key, Value> {
    using type = std::map<Key, Value>;
};




// 顶点属性
struct empty_node_prop {}; // NodeProp 为void时的空类

template<typename NodeType, typename NodePropType>
struct NodePropStorageSelector {
    using type = std::unordered_map<NodeType, NodePropType>;
};

template<typename NodeType>
struct NodePropStorageSelector<NodeType, void> {
    using type = empty_node_prop;
};


// 适配边属性存储的哈希组合函数
template<typename T1, typename T2>
struct PairHash {
    std::size_t operator()(const std::pair<T1, T2>& p) const noexcept {
        std::size_t h1 = std::hash<T1>{}(p.first);
        std::size_t h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2)); // 用位移 + 异或简单合并
    }
};


// 边属性
struct empty_edge_prop {}; // EdgeProp 为void时的空类

template<typename NodeType, typename EdgePropType, MultiEdge multi>
struct EdgePropStorageSelector {
	using Pair = std::pair<NodeType, NodeType>;
	using Hash = PairHash<NodeType, NodeType>;
    using type = std::unordered_map<Pair, EdgePropType, Hash>;
};

template<typename NodeType, typename EdgePropType, MultiEdge multi>
struct EdgePropStorageSelector<NodeType, EdgePropType, MultiEdge::ALLOWED> {
	using Pair = std::pair<NodeType, NodeType>;
	using Hash = PairHash<NodeType, NodeType>;
    using type = std::unordered_multimap<Pair, EdgePropType, Hash>;
};

template<typename NodeType,MultiEdge multi>
struct EdgePropStorageSelector<NodeType, void, multi> {
    using type = empty_edge_prop;
};


// 顶点查找返回值
template <typename NodeType, typename NodePropType>
struct NodeInfo {
    const NodeType& node;   // 节点标识
    const NodePropType* prop;   // 可为 nullptr
};

// 边查找返回值
template <typename NodeType, typename EdgePropType>
struct NodeInfo {
    const NodeType& from;
	const NodeType& to;
    const EdgePropType* prop;
	int count;
	bool directed;
};

// 可比较
template<typename T, typename = void>
struct is_less_comparable : std::false_type {};

template<typename T>
struct is_less_comparable<
    T,
    std::void_t<decltype(std::declval<T>() < std::declval<T>())
    >
> : std::true_type {};

template<typename T>
constexpr bool is_less_comparable_v = is_less_comparable<T>::value;


// 类定义

template<typename NodeType=int, typename NodePropType=void, typename EdgePropType=void,
    EdgeDirection edge_direction=EdgeDirection::UNDIRECTED,
    MultiEdge multi_edge=MultiEdge::DISALLOWED,
    SelfLoop self_loop=SelfLoop::DISALLOWED,
    Map adj_list_spec=Map::UNORDERED_MAP,
    Container neighbors_container_spec=Container::UNORDERED_SET>

class Graph{
private:
	// 邻接表
    using NeighborContainer = typename NeighborContainerSelector<neighbors_container_spec, NodeType>::type;
	using AdjList = typename AdjListSelector<adj_list_spec, NodeType, NeighborContainer>::type;

	AdjList adj_list;

	// 顶点属性和边属性
	using NodeProps = typename NodePropStorageSelector<NodeType, NodePropType>::type;
	using EdgeProps = typename EdgePropStorageSelector<NodeType, EdgePropType, multi_edge>::type;

	NodeProps node_props;
	EdgeProps edge_props;

private:
	//类内参数常量
	static constexpr EdgeDirection direction = edge_direction;
    static constexpr MultiEdge multi = multi_edge;
    static constexpr SelfLoop selfloop = self_loop;

public:
	Graph(){
		static_assert(is_less_comparable_v<NodeType>,"顶点类型必须可比较!");

	}

    // 添加无属性结点
	int add_node(const NodeType& node){
		static_assert((std::is_same_v<NodePropType,void>),"此图必须添加顶点属性!");
		bool inserted = adj_list.try_emplace(node).second;
		return inserted;
	}
	
    template <typename... Args>
    int add_node(const NodeType& node,const Args&... rest_nodes){
		static_assert((std::is_same_v<NodePropType,void>),"此图必须添加顶点属性!");
		static_assert((std::is_same_v<NodeType,Args> && ...),"请保持添加的顶点具有正确的类型!");
		int count=0;
		count+=add_node(node);
		if constexpr(sizeof...(Args)>0) {
			count += add_node(rest_nodes...);
		}
		return count;
	}


	// 添加有属性顶点
	int add_node_with_prop(const NodeType& node,const NodePropType& nodeprop){
		static_assert(!(std::is_same_v<NodePropType,void>),"此图不能添加顶点属性!");
		bool inserted = adj_list.try_emplace(node).second;
		if (inserted) {
			node_props.emplace(node,nodeprop);
		}
		return inserted;
	}

	template <typename... Args>
	int add_node_with_prop(const std::pair<NodeType,NodePropType>& node,const Args&... rest_nodes){
		static_assert((std::is_same_v<NodePropType,void>),"此图不能添加顶点属性!");
		static_assert((std::is_same_v<NodeType,Args> && ...),"请保持添加的顶点具有正确的类型!");
		int count=0;
		count+=add_node_with_prop(node);
		if constexpr(sizeof...(Args)>0) {
			count += add_node(rest_nodes...);
		}
		return count;
	}


    // 添加无属性边
	int add_edge(const NodeType& outnode, const NodeType& innode) {
		static_assert((std::is_same_v<EdgePropType,void>),"此图必须添加边属性!");
		auto it_out = adj_list.find(outnode),it_in  = adj_list.find(innode);

		if (it_out == adj_list.end() || it_in == adj_list.end()) return 0;
		
		if constexpr (selfloop == SelfLoop::DISALLOWED) { // 自环配置
			if (outnode == innode) return 0;
		}

		if constexpr (multi == MultiEdge::DISALLOWED) { // 多重边配置
			auto& neigh_out = it_out->second;
			if constexpr (neighbors_container_spec == Container::VEC || neighbors_container_spec == Container::LIST) {
				if (std::find(neigh_out.begin(), neigh_out.end(),innode) != neigh_out.end()) return 0; // 顺序容器：用 std::find
			} else {
				if (neigh_out.find(innode) != neigh_out.end()) return 0; // 关联容器：用成员 find
			}
		}
		
		if constexpr (neighbors_container_spec == Container::VEC || neighbors_container_spec == Container::LIST) { // 邻居容器配置
			it_out->second.emplace_back(innode);
			if constexpr (direction == EdgeDirection::UNDIRECTED) { // 无向图：再插入 in -> out
				if (innode!=outnode) it_in->second.emplace_back(outnode);
			}
		} else {
			it_out->second.emplace(innode);
			if constexpr (direction == EdgeDirection::UNDIRECTED) {
				if (innode!=outnode) it_in->second.emplace_back(outnode);
			}
		}
		
		return 1;
	}

    int add_edge(const NodeType& outnode,const std::initializer_list<NodeType>& innodes){
		static_assert((std::is_same_v<EdgePropType,void>),"此图必须添加边属性!");
		auto it_out = adj_list.find(outnode);
		if (it_out == adj_list.end()) return 0;
		int count = 0;
		for(auto innode : innodes){
			auto it_in  = adj_list.find(innode);
			if (it_in == adj_list.end()) continue;

			if constexpr (selfloop == SelfLoop::DISALLOWED) { // 自环配置
				if (outnode == innode) continue;
			}

			if constexpr (multi == MultiEdge::DISALLOWED) { // 多重边配置
				auto& neigh_out = it_out->second;
				if constexpr (neighbors_container_spec == Container::VEC || neighbors_container_spec == Container::LIST) {
					if (std::find(neigh_out.begin(), neigh_out.end(),innode) != neigh_out.end()) continue; // 顺序容器：用 std::find
				} else {
					if (neigh_out.find(innode) != neigh_out.end()) continue; // 关联容器：用成员 find
				}
			}

			count++;
			if constexpr (neighbors_container_spec == Container::VEC || neighbors_container_spec == Container::LIST) { // 邻居容器配置
				it_out->second.emplace_back(innode);
				if constexpr (direction == EdgeDirection::UNDIRECTED) { // 无向图：再插入 in -> out
					if (innode!=outnode) it_in->second.emplace_back(outnode);
				}
			} else {
				it_out->second.emplace(innode);
				if constexpr (direction == EdgeDirection::UNDIRECTED) {
					if (innode!=outnode) it_in->second.emplace(outnode);
				}
			}
		}
		return count;
	}

    int add_edge(const std::initializer_list<NodeType>& outnodes,const NodeType& innode){
		static_assert((std::is_same_v<EdgePropType,void>),"此图必须添加边属性!");
		auto it_in = adj_list.find(innode);
		if(it_in == adj_list.end()) return 0;
		int count=0;
		for(auto outnode : outnodes){
			auto it_out = adj_list.find(outnode);
			if (it_out == adj_list.end()) continue;
			
			if constexpr (selfloop == SelfLoop::DISALLOWED) { // 自环配置
				if (outnode == innode) continue;
			}

			if constexpr (multi == MultiEdge::DISALLOWED) { // 多重边配置
				auto& neigh_out = it_out->second;
				if constexpr (neighbors_container_spec == Container::VEC || neighbors_container_spec == Container::LIST) {
					if (std::find(neigh_out.begin(), neigh_out.end(),innode) != neigh_out.end()) continue; // 顺序容器：用 std::find
				} else {
					if (neigh_out.find(innode) != neigh_out.end()) continue; // 关联容器：用成员 find
				}
			}

			count++;
			if constexpr (neighbors_container_spec == Container::VEC || neighbors_container_spec == Container::LIST) { // 邻居容器配置
				it_out->second.emplace_back(innode);
				if constexpr (direction == EdgeDirection::UNDIRECTED) { // 无向图：再插入 in -> out
					if (innode!=outnode) it_in->second.emplace_back(outnode);
				}
			} else {
				it_out->second.emplace(innode);
				if constexpr (direction == EdgeDirection::UNDIRECTED) {
					if (innode!=outnode) it_in->second.emplace(outnode);
				}
			}
		}
		return count;
	}

	// 添加带属性边
	int add_edge_with_prop(const NodeType& outnode, const NodeType& innode, const EdgePropType& edgeprop){
		static_assert(!(std::is_same_v<EdgePropType,void>),"此图不能添加边属性!");
		auto it_out = adj_list.find(outnode),it_in  = adj_list.find(innode);

		if (it_out == adj_list.end() || it_in == adj_list.end()) return 0;
		
		if constexpr (selfloop == SelfLoop::DISALLOWED) { // 自环配置
			if (outnode == innode) return 0;
		}

		if constexpr (multi == MultiEdge::DISALLOWED) { // 多重边配置
			auto& neigh_out = it_out->second;
			if constexpr (neighbors_container_spec == Container::VEC || neighbors_container_spec == Container::LIST) {
				if (std::find(neigh_out.begin(), neigh_out.end(),innode) != neigh_out.end()) return 0; // 顺序容器：用 std::find
			} else {
				if (neigh_out.find(innode) != neigh_out.end()) return 0; // 关联容器：用成员 find
			}
		}
		
		if constexpr (neighbors_container_spec == Container::VEC || neighbors_container_spec == Container::LIST) { // 邻居容器配置
			it_out->second.emplace_back(innode);
			if constexpr (direction == EdgeDirection::UNDIRECTED) { // 无向图：再插入 in -> out
				if (innode!=outnode) it_in->second.emplace_back(outnode);
			}
		} else {
			it_out->second.emplace(innode);
			if constexpr (direction == EdgeDirection::UNDIRECTED) {
				if (innode!=outnode) it_in->second.emplace(outnode);
			}
		}
		if constexpr (direction == EdgeDirection::UNDIRECTED){
			edge_props.emplace({std::min(outnode,innode),std::max(outnode,innode)},edgeprop); // 这里只记录了一个方向的边
		}else{
			edge_props.emplace({outnode,innode},edgeprop);
		} 
		return 1;
	}

    // 删除结点
	int remove_node(const NodeType& node) {
		auto it = adj_list.find(node);
		if (it == adj_list.end()) {
			return 0;
		}
		if constexpr (!std::is_same_v<EdgePropType, void>) {
			std::unordered_set<NodeType> regarding_nodes;
			// 删除其它节点对它的引用
			for (auto& [u, neigh] : adj_list) {
				if (u == node) continue;
				if constexpr (neighbors_container_spec == Container::VEC || neighbors_container_spec == Container::LIST) {
					auto erase_start = std::remove(neigh.begin(), neigh.end(), node);
					if(erase_start != neigh.end()) regarding_nodes.emplace(u);
					neigh.erase(erase_start, neigh.end());
				} else {
					if (neigh.erase(node)>0) regarding_nodes.emplace(u);
				}
			}

			// 删除相关边
			if constexpr (direction == EdgeDirection::UNDIRECTED){
				for(auto& regarding_node : regarding_nodes){
					edge_props.erase(std::make_pair(std::min(regarding_node,node), std::max(regarding_node,node)));
				}
			}else{
				for(auto& regarding_node : regarding_nodes){ //有向图正反都要删除
					edge_props.erase(std::make_pair(regarding_node,node));
					edge_props.erase(std::make_pair(node,regarding_node));
				}
			}

		}else{
			for (auto& [u, neigh] : adj_list) {
				if (u == node) continue;
				if constexpr (neighbors_container_spec == Container::VEC || neighbors_container_spec == Container::LIST) {
					neigh.erase(std::remove(neigh.begin(), neigh.end(), node), neigh.end());
				} else {
					neigh.erase(node);
				}
			}
		}

		// 若存在节点属性，则同步删除
		if constexpr (!std::is_same_v<NodePropType, void>) {
			node_props.erase(node);
		}

		adj_list.erase(it);
		return 1;
	}

    template <typename... Args>
	int remove_node(const NodeType& node, const Args&... rest_nodes) {
		static_assert((std::is_same_v<NodeType, Args> && ...),"请确认删除的顶点具有正确的类型!");
		int count = remove_node(node);
		if constexpr (sizeof...(Args) > 0) {
			count += remove_node(rest_nodes...);
		}
		return count;
	}

	// 删除边(自环？)
    int remove_edge(const NodeType& outnode,const NodeType& innode){
		auto it_out = adj_list.find(outnode);
		auto it_in  = adj_list.find(innode);
		if (it_out == adj_list.end() || it_in == adj_list.end()) return 0;

		if constexpr (multi == MultiEdge::DISALLOWED) {
			auto& neigh_out = it_out->second;
			if constexpr (neighbors_container_spec == Container::VEC || neighbors_container_spec == Container::LIST) { // 邻居容器配置
				auto where_in = std::find(neigh_out.begin(),neigh_out.end(),innode);
				if(where_in == neigh_out.end()) return 0;
				neigh_out.erase(where_in);
				if constexpr (direction == EdgeDirection::UNDIRECTED) { 
					auto& neigh_in = it_in->second;
					neigh_in.erase(std::find(neigh_in.begin(),neigh_in.end(),outnode));
				}
				if constexpr (!std::is_same_v<EdgePropType, void>){
					if constexpr (direction == EdgeDirection::UNDIRECTED) {
						edge_props.erase(std::make_pair(std::min(outnode, innode),std::max(outnode, innode)));
					} else {
						edge_props.erase(std::make_pair(outnode, innode));
					}
				}
				return 1;
			} else {
				int removed = neigh_out.erase(innode); // 删 out -> innode 这一条
				if constexpr (direction == EdgeDirection::UNDIRECTED) {
					it_in->second.erase(outnode); // 无向图：同步删 in -> out
				}
				if constexpr (!std::is_same_v<EdgePropType, void>){
					if constexpr (direction == EdgeDirection::UNDIRECTED) {
						edge_props.erase(std::make_pair(std::min(outnode, innode),std::max(outnode, innode)));
					} else {
						edge_props.erase(std::make_pair(outnode, innode));
					}
				}
				return removed;
			}
		} else {  // 可重复边
			size_t removed;
			auto& neigh_out = it_out->second;
			if constexpr (neighbors_container_spec == Container::VEC || neighbors_container_spec == Container::LIST) { // 邻居容器配置
				auto where_in = std::remove(neigh_out.begin(),neigh_out.end(),innode);
				if(where_in == neigh_out.end()) return 0;
				removed = std::distance(where_in,neigh_out.end());
				neigh_out.erase(where_in,neigh_out.end());
				if constexpr (direction == EdgeDirection::UNDIRECTED) { 
					auto& neigh_in = it_in->second;
					neigh_in.erase(std::remove(neigh_in.begin(),neigh_in.end(),outnode),neigh_in.end());
				}
				if constexpr (!std::is_same_v<EdgePropType, void>){
					if constexpr (direction == EdgeDirection::UNDIRECTED) {
						edge_props.erase(std::make_pair(std::min(outnode, innode),std::max(outnode, innode)));
					} else {
						edge_props.erase(std::make_pair(outnode, innode));
					}
				}
				return removed;
			} else {
				removed = neigh_out.erase(innode); // 删 out -> innode
				if constexpr (direction == EdgeDirection::UNDIRECTED) {
					it_in->second.erase(outnode); // 无向图：同步删 in -> out
				}
				if constexpr (!std::is_same_v<EdgePropType, void>){
					if constexpr (direction == EdgeDirection::UNDIRECTED) {
						edge_props.erase(std::make_pair(std::min(outnode, innode),std::max(outnode, innode)));
					} else {
						edge_props.erase(std::make_pair(outnode, innode));
					}
				}
				return removed;
			}
		}
	} 

	int remove_edge_with_prop(const NodeType& outnode, const NodeType& innode, const EdgePropType& edgeprop){
		static_assert(!(std::is_same_v<EdgePropType,void>),"此图不存在边属性!");
		auto it_out = adj_list.find(outnode);
		auto it_in  = adj_list.find(innode);
		if (it_out == adj_list.end() || it_in == adj_list.end()) return 0;

		if constexpr (multi == MultiEdge::DISALLOWED) {
			auto& neigh_out = it_out->second;
			if constexpr (neighbors_container_spec == Container::VEC || neighbors_container_spec == Container::LIST) { // 邻居容器配置
				auto where_in = std::find(neigh_out.begin(),neigh_out.end(),innode);
				if(where_in == neigh_out.end()) return 0;

				// 从边属性表里删除边
				EdgeProps::iterator edge_loc;
				if constexpr (direction == EdgeDirection::UNDIRECTED) {
					edge_loc=edge_props.find(std::make_pair(std::min(outnode, innode),std::max(outnode, innode)));
				} else {
					edge_loc=edge_props.find(std::make_pair(outnode, innode));
				}
				if(edge_loc == edge_props.end() || edge_loc->second != edgeprop) return 0;
				edge_props.erase(edge_loc);

				neigh_out.erase(where_in);
				if constexpr (direction == EdgeDirection::UNDIRECTED) { 
					auto& neigh_in = it_in->second;
					neigh_in.erase(std::find(neigh_in.begin(),neigh_in.end(),outnode));
				}

				return 1;
			} else {
				EdgeProps::iterator edge_loc;
				if constexpr (direction == EdgeDirection::UNDIRECTED) {
					edge_loc=edge_props.find(std::make_pair(std::min(outnode, innode),std::max(outnode, innode)));
				} else {
					edge_loc=edge_props.find(std::make_pair(outnode, innode));
				}
				if(edge_loc == edge_props.end() || edge_loc->second != edgeprop) return 0;
				edge_props.erase(edge_loc);

				neigh_out.erase(innode);
				if constexpr (direction == EdgeDirection::UNDIRECTED) {
					it_in->second.erase(outnode);
				}

				return 1;
			}
		} else {  // 可重复边
			size_t removed=0;

			std::pair<EdgeProps::iterator,EdgeProps::iterator> check_edge;
			if constexpr (direction == EdgeDirection::UNDIRECTED) { // 删边表
				check_edge = edge_props.equal_range(std::make_pair(std::min(outnode, innode),std::max(outnode, innode)));
			} else {
				check_edge = edge_props.equal_range(std::make_pair(outnode, innode));
			}
			for (auto it=check_edge.first;it!=check_edge.second;){
				if (it->second == edgeprop){
					it = edge_props.erase(it);
					removed++;
				}else{
					++it;
				}
			}

			auto& neigh_out = it_out->second;
			if constexpr (neighbors_container_spec == Container::VEC || neighbors_container_spec == Container::LIST) { // 邻居容器配置
				auto rit=neigh_out.rbegin();
				size_t i=0;
				while(i < removed && rit != neigh_out.rend()){
					if(*rit == innode){
						auto it = rit.base()-1;
						it = neigh_out.erase(it);
						rit=std::reverse_iterator<decltype(it)>(it);
						i++;
					}
					else{
						++rit;
					}
				}
				if constexpr (direction == EdgeDirection::UNDIRECTED) { 
					auto& neigh_in = it_in->second;
					auto rit=neigh_in.rbegin();
					i=0;
					while(i < removed && rit != neigh_in.rend()){
						if(*rit == outnode){
							auto it = rit.base()-1;
							it = neigh_in.erase(it);
							rit=std::reverse_iterator<decltype(it)>(it);
							i++;
						}
						else{
							++rit;
						}
					}
				}
			} else {
				auto& check_out = neigh_out.equal_range(innode);
				auto it=check_out.first;
				size_t i=0;
				while (it != check_out.second && i < removed) {
					it = neigh_out.erase(it);
					++i;
				}
				if constexpr (direction == EdgeDirection::UNDIRECTED) {
					auto& neigh_in=it_in->second;
					auto& check_in = neigh_in.equal_range(outnode);
					auto it=check_in.first;
					i=0;
					while (it != check_in.second && i < removed) {
						it = neigh_in.erase(it);
						++i;
					}
				}
			}
			return removed;
		}
	}

	// 查找结点
    bool has_node(const NodeType& node) const {return adj_list.find(node) != adj_list.end();}

    std::optional<NodeInfo<NodeType,NodePropType>> find_node(const NodeType& node) const {
		if constexpr(std::is_same_v<NodePropType,void>){
			auto& findnode = adj_list.find(node);
			return (findnode==adj_list.end())? std::nullopt : NodeInfo<NodeType,NodePropType>{node,nullptr};
		} else {
			auto& findnode = node_props.find(node);
			return (findnode==adj_list.end())? std::nullopt : NodeInfo<NodeType,NodePropType>{node,&(findnode->second)};
		}
	}

	// 查找边
	int has_edge(const NodeType& outnode,const NodeType& innode) const{
		if constexpr(std::is_same_v<EdgePropType,void>) {
			auto& neigh_out = adj_list.find(outnode);
			if constexpr (neighbors_container_spec == Container::VEC || neighbors_container_spec == Container::LIST){
				return std::count(neigh_out.begin(),neigh_out.end(),innode);
			} else {
				return neigh_out->count(innode);
			}
		} else {
			if constexpr (direction == EdgeDirection::UNDIRECTED){
				return edge_props.count(std::make_pair(std::min(outnode,innode),std::max(outnode,innode)));
			}else{
				return edge_props.count(std::make_pair(outnode,innode));
			}
		}
	}

	int has_node(){
		
	}

};


#endif