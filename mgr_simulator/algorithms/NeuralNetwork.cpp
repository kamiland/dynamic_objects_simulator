#include "NeuralNetwork.hpp"

NeuralNetwork::NeuralNetwork()
{
    /**
     * PSEUDOCODE
    */

    // for(ii in range(1, self.layers_count))
    // {
    //     biases.append(np.zeros(self.nodes_count[ii]))
    //     deltas.append(np.zeros(self.nodes_count[ii]))
    //     weights.append(np.zeros((self.nodes_count[ii], self.nodes_count[ii - 1])))
    // }

    // for(ii in range(self.layers_count))
    // {
    //     layers.append(np.zeros(self.nodes_count[ii]))
    //     z_layers.append(np.zeros(self.nodes_count[ii]))
    //     dz_layers.append(np.zeros(self.nodes_count[ii]))
    //     deltas_layers.append(np.zeros(self.nodes_count[ii]))
    // }
}

NeuralNetwork::~NeuralNetwork()
{
}

double NeuralNetwork::Feedforward()
{

    /**
     * PSEUDOCODE
    */
    
    // self.layers[0] = _input

    // // loop through layers
    // for(layer in range(self.layers_count - 1))
    // {
    //     // loop through nodes
    //     for(node_next in range(self.nodes_count[layer + 1]))
    //     {
    //         temp = 0
    //         // calculate value for each node
    //         for(node_current in range(self.nodes_count[layer]))
    //         {
    //             temp = temp + self.layers[layer][node_current] * self.weights[layer][node_next][node_current]
    //         }   

    //         self.layers[layer + 1][node_next] = temp + self.biases[layer][node_next]
    //         self.z_layers[layer + 1][node_next] = self.layers[layer + 1][node_next]
    //     }

    //     // activation
    //     if(self.activation_functions[layer] == 'relu')
    //     {
    //         for(node in range(self.layers[layer + 1].__len__()))
    //         {
    //             if(self.layers[layer + 1][node] < 0)
    //             {
    //                 self.layers[layer + 1][node] = 0
    //             }
    //             if(self.z_layers[layer + 1][node] > 0)
    //             {
    //                 self.dz_layers[layer + 1][node] = 1
    //             }
    //             else
    //             {
    //                 self.dz_layers[layer + 1][node] = 0
    //             }
    //         }
    //     }
    //     else if(self.activation_functions[layer] == 'log')
    //     {
    //         for(node in range(self.layers[layer + 1].__len__()))
    //         {
    //             self.layers[layer + 1][node] = logistic(self.layers[layer + 1][node])
    //             self.dz_layers[layer + 1][node] = logistic_d(self.z_layers[layer + 1][node])
    //         }
    //     }
    //     else if(self.activation_functions[layer] == 'none')
    //     {
    //         for(node in range(self.layers[layer + 1].__len__()))
    //         {
    //             self.layers[layer + 1][node] = none(self.layers[layer + 1][node])
    //         }
    //     }
    //     else
    //     {
    //         cout << "Not supported activation function" << endl;
    //     }
    // }
    // self.output = self.layers[-1][-1]
    // return self.layers[-1]
}