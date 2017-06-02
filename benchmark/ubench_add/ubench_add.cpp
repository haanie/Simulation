//====== Graph Benchmark Suites ======//
//======= RandomGraph Construction =======//
//
// Usage: ./randomgraph --vertex <vertex #> --edge <edge #>

#include "common.h"
#include "def.h"
#include "perf.h"
#include "openG.h"

using namespace std;

#define SEED 111

class vertex_property
{
public:
    vertex_property():value(0){}
    vertex_property(uint64_t x):value(x){}

    uint64_t value;
};
class edge_property
{
public:
    edge_property():value(0){}
    edge_property(uint64_t x):value(x){}

    uint64_t value;
};

typedef openG::extGraph<vertex_property, edge_property> graph_t;
typedef graph_t::vertex_iterator    vertex_iterator;
typedef graph_t::edge_iterator      edge_iterator;

//==============================================================//
void arg_init(argument_parser & arg)
{
    arg.add_arg("vertex","100","vertex #");
    arg.add_arg("edge","1000","edge #");
}
//==============================================================//

void randomgraph_construction(graph_t &g, size_t vertex_num, size_t edge_num)
{
    for (size_t i=0;i<vertex_num;i++) 
    {
        vertex_iterator vit = g.add_vertex();
        vit->set_property(vertex_property(i));
    }
    for (size_t i=0;i<edge_num;i++) 
    {
        edge_iterator eit;
        if (g.add_edge(rand()%vertex_num, rand()%vertex_num, eit))
            eit->set_property(edge_property(i));
    }
}



//==============================================================//

void output(graph_t& g)
{
    cout<<"BFS Results: \n";
    vertex_iterator vit;
    for (vit=g.vertices_begin(); vit!=g.vertices_end(); vit++)
    {
        cout<<"== vertex "<<vit->id()<<": edge#-"<<vit->edges_size()<<"\n";
    }
}

//==============================================================//
int main(int argc, char * argv[])
{
    graphBIG::print();
    cout<<"Benchmark: ubench-add\n";

    argument_parser arg;
    gBenchPerf_event perf;
    arg_init(arg);
    if (arg.parse(argc,argv,perf,false)==false)
    {
        arg.help();
        return -1;
    }
    
    size_t vertex_num,edge_num;
    arg.get_value("vertex",vertex_num);
    arg.get_value("edge",edge_num);

    srand(SEED); // fix seed to avoid runtime dynamics
    graph_t g;
    double t1, t2;
    
    cout<<"== "<<vertex_num<<" vertices  "<<edge_num<<" edges\n";

    t1 = timer::get_usec();
    perf.open();
    perf.start();

    randomgraph_construction(g, vertex_num, edge_num);

    perf.stop();
    t2 = timer::get_usec();
    cout<<"\nadd finish: \n";
    cout<<"== "<<g.num_vertices()<<" vertices  "<<g.num_edges()<<" edges\n";

#ifndef ENABLE_VERIFY
    cout<<"== time: "<<t2-t1<<" sec\n";
    perf.print();
#else
    (void)t1;
    (void)t2;
#endif

#ifdef ENABLE_OUTPUT
    cout<<"\n";
    output(g);
#endif

    cout<<"==================================================================\n";
    return 0;
}  // end main

