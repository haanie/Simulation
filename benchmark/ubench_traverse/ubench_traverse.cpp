//====== Graph Benchmark Suites ======//
//======= RandomGraph Construction =======//
//
// Usage: ./graphupdate --delete <vertex #> --dataset <dataset path>

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

//==============================================================//

void graph_traverse(graph_t& g)
{
    vertex_iterator vit;
    uint64_t vcnt=0;
    uint64_t ecnt=0;
    for (vit=g.vertices_begin(); vit!=g.vertices_end(); vit++)
    {
        vit->set_property(vertex_property(vcnt++));
        edge_iterator eit;
        for (eit=vit->out_edges_begin();eit!=vit->out_edges_end();eit++)
        {
            eit->set_property(edge_property(ecnt++));
        }
    }
}
//==============================================================//
void output(graph_t& g)
{
    cout<<"Results: \n";
    vertex_iterator vit;
    for (vit=g.vertices_begin(); vit!=g.vertices_end(); vit++)
    {
        cout<<"== vertex "<<vit->id()<<": order "<<vit->property().value<<"\n";
    }
}
//==============================================================//
int main(int argc, char * argv[])
{
    graphBIG::print();
    cout<<"Benchmark: ubench-traverse\n";

    argument_parser arg;
    gBenchPerf_event perf;
    if (arg.parse(argc,argv,perf,false)==false)
    {
        arg.help();
        return -1;
    }
    string path, separator;
    arg.get_value("dataset",path);
    arg.get_value("separator",separator);

    graph_t g;
    double t1, t2;
    
    cout<<"loading data... \n";

    t1 = timer::get_usec();
    string vfile = path + "/vertex.csv";
    string efile = path + "/edge.csv";

#ifndef EDGES_ONLY
    if (g.load_csv_vertices(vfile, true, separator, 0) == -1)
        return -1;
    if (g.load_csv_edges(efile, true, separator, 0, 1) == -1) 
        return -1;
#else
    if (g.load_csv_edges(efile, true, separator, 0, 1) == -1)
        return -1;
#endif

    size_t vertex_num = g.num_vertices();
    size_t edge_num = g.num_edges();
    t2 = timer::get_usec();

    cout<<"== "<<vertex_num<<" vertices  "<<edge_num<<" edges\n";
    
#ifndef ENABLE_VERIFY
    cout<<"== time: "<<t2-t1<<" sec\n";
#endif
    t1 = timer::get_usec();
    perf.start();

    graph_traverse(g);


    perf.stop();
    t2 = timer::get_usec();
    cout<<"\ngraph traverse finish: \n";
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

