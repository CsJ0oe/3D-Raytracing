#include "bvh.h"
#include "mesh.h"
#include <iostream>

void BVH::build(const Mesh* pMesh, int targetCellSize, int maxDepth)
{
    // store a pointer to the mesh
    m_pMesh = pMesh;
    // allocate the root node
    m_nodes.resize(1);

    if(m_pMesh->nbFaces() <= targetCellSize) { // only one node
        m_nodes[0].box = pMesh->AABB();
        m_nodes[0].first_face_id = 0;
        m_nodes[0].is_leaf = true;
        m_nodes[0].nb_faces = m_pMesh->nbFaces();
        m_faces.resize(m_pMesh->nbFaces());
        for(int i=0; i<m_pMesh->nbFaces(); ++i)
        {
            m_faces[i] = i;
        }
    }else{
        // TOFIX reserve space for other nodes to avoid multiple memory reallocations
        //m_nodes.reserve( std::min<int>(2<<maxDepth, 2*std::log(m_pMesh->nbFaces()/targetCellSize)) );
        m_nodes.reserve(2<<maxDepth);

        // compute centroids and initialize the face list
        m_centroids.resize(m_pMesh->nbFaces());
        m_faces.resize(m_pMesh->nbFaces());
        for(int i=0; i<m_pMesh->nbFaces(); ++i)
        {
            m_centroids[i] = (m_pMesh->vertexOfFace(i, 0).position + m_pMesh->vertexOfFace(i, 1).position + m_pMesh->vertexOfFace(i, 2).position)/3.f;
            m_faces[i] = i;
        }

        // recursively build the BVH, starting from the root node and the entire list of faces
        buildNode(0, 0, m_pMesh->nbFaces(), 0, targetCellSize, maxDepth);
    }
}

bool BVH::intersect(const Ray& ray, Hit& hit) const
{
    // compute the intersection with the root node
    float tMin, tMax;
    Normal3f n;
    ::intersect(ray, m_nodes[0].box, tMin, tMax, n);

    // vérifier si on a bien une intersection (en fonction de tMin, tMax, et hit.t()), et si oui appeler intersecNode...
    if( ::intersect(ray, m_nodes[0].box, tMin, tMax, n) && tMin<=hit.t())
        return intersectNode(0, ray, hit);

    return false;
}

bool BVH::intersectNode(int nodeId, const Ray& ray, Hit& hit) const
{
    bool result = false;
    const Node& node = m_nodes[nodeId];

    // deux cas:
    if (node.is_leaf) {
        // soit mNodes[nodeId] est une feuille (il faut alors intersecter les triangles du noeud),
        for (int f=node.first_face_id; f<(node.first_face_id+node.nb_faces); ++f) {
            result |= m_pMesh->intersectFace(ray, hit, f);
        }
    } else {
        // soit c'est un noeud interne (il faut visiter les fils (ou pas))
        float tMin1, tMax1,tMin2, tMax2;
        Normal3f n1, n2;
        if( ::intersect(ray, m_nodes[node.first_child_id].box, tMin1, tMax1, n1)  )
            result |= intersectNode(node.first_child_id, ray, hit);
        if( ::intersect(ray, m_nodes[node.first_child_id+1].box, tMin2, tMax2, n2) )
            result |= intersectNode(node.first_child_id+1, ray, hit);
    }

    return result;
}

/** Sorts the faces with respect to their centroid along the dimension \a dim and spliting value \a split_value.
  * \returns the middle index
  */
int BVH::split(int start, int end, int dim, float split_value)
{
    int l(start), r(end-1);
    while(l<r)
    {
        // find the first on the left
        while(l<end && m_centroids[l](dim) < split_value) ++l;
        while(r>=start && m_centroids[r](dim) >= split_value) --r;
        if(l>r) break;
        std::swap(m_centroids[l], m_centroids[r]);
        std::swap(m_faces[l], m_faces[r]);
        ++l;
        --r;
    }
    return m_centroids[l][dim]<split_value ? l+1 : l;
}

void BVH::buildNode(int nodeId, int start, int end, int level, int targetCellSize, int maxDepth)
{
    Node& node = m_nodes[nodeId];

    // étape 1 : calculer la boite englobante des faces indexées de m_faces[start] à m_faces[end]
    // (Utiliser la fonction extend de Eigen::AlignedBox3f et la fonction m_pMesh->vertexOfFace(int) pour obtenir les coordonnées des sommets des faces)
    node.nb_faces = end - start;
    for (int i=start; i<end; ++i) {
        node.box.extend(m_pMesh->vertexOfFace(m_faces[i], 0).position);
        node.box.extend(m_pMesh->vertexOfFace(m_faces[i], 1).position);
        node.box.extend(m_pMesh->vertexOfFace(m_faces[i], 2).position);
    }

    // étape 2 : déterminer si il s'agit d'une feuille (appliquer les critères d'arrêts)
    if(node.nb_faces <= targetCellSize || level >= (maxDepth-1)) {
        // Si c'est une feuille, finaliser le noeud et quitter la fonction
        node.first_face_id = start;
        node.is_leaf = true;
        cout << "leaf - " << node.nb_faces << " / " << level << endl;
        return;
    }

    // Si c'est un noeud interne :

    // étape 3 : calculer l'index de la dimension (x=0, y=1, ou z=2) et la valeur du plan de coupe
    // (on découpe au milieu de la boite selon la plus grande dimension)
    auto sizes = node.box.sizes();
    int index = 0;
    index = (sizes[1]<sizes[index])?index:1;
    index = (sizes[2]<sizes[index])?index:2;

    // étape 4 : appeler la fonction split pour trier (partiellement) les faces et vérifier si le split a été utile
    float split_value = (node.box.min()[index] + node.box.max()[index]) / 2.0f;
    int mid = split(start, end, index, split_value);
    if (mid == start || mid == end) mid = (start + end) / 2;

    // étape 5 : allouer les fils, et les construire en appelant buildNode...
    node.first_child_id = m_nodes.size();
    m_nodes.push_back(Node());
    m_nodes.push_back(Node());
    buildNode(node.first_child_id, start, mid, level+1, targetCellSize, maxDepth);
    buildNode(node.first_child_id+1, mid, end, level+1, targetCellSize, maxDepth);
}
