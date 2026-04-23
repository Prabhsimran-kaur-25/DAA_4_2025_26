const express = require('express');
const app = express();
app.use(express.json());
app.use(express.static(__dirname));

let graph = {  "Alice": ["Bob", "Charlie"],
  "Bob": ["Alice", "David", "Eve"],
  "Charlie": ["Alice", "David"],
  "David": ["Bob", "Charlie", "Eve"],
  "Eve": ["Bob", "David", "Frank"],
  "Frank": ["Eve", "Grace"],
  "Grace": ["Frank"]};

function addUser(u){
    if(!graph[u]) graph[u] = [];
}

function addFriend(u, v){
    if(!graph[u]) graph[u] = [];
    if(!graph[v]) graph[v] = [];

    if(!graph[u].includes(v)) graph[u].push(v);
    if(!graph[v].includes(u)) graph[v].push(u);
}

function mutual(u, v){
    if(!graph[u] || !graph[v]) return [];
    let set = new Set(graph[u]);
    return graph[v].filter(x => set.has(x));
}

function shortestPath(src,dest){
    if(!graph[src] || !graph[dest]) return [];
    let q=[src], vis=new Set([src]), parent={};
    while(q.length){
        let cur=q.shift();
        if(cur===dest) break;
        for(let n of graph[cur]){
            if(!vis.has(n)){
                vis.add(n);
                parent[n]=cur;
                q.push(n);
            }
        }
    }
    let path=[dest];
    while(path[0]!==src){
        if(!parent[path[0]]) return [];
        path.unshift(parent[path[0]]);
    }
    return path;
}

app.post('/addUser',(req,res)=>{
    addUser(req.body.user);
    res.send("ok");
});

app.post('/addFriend',(req,res)=>{
    addFriend(req.body.u,req.body.v);
    res.send("ok");
});

app.get('/mutual',(req,res)=>{
    res.json(mutual(req.query.u,req.query.v));
});

app.get('/recommend',(req,res)=>{
    let user = req.query.u;
    let score = {};

    if(!graph[user]) return res.json([]);

    graph[user].forEach(friend=>{
        graph[friend].forEach(ff=>{
            if(ff !== user && !graph[user].includes(ff)){
                if(!score[ff]) score[ff] = 0;
                score[ff]++;
            }
        });
    });

    let result = Object.keys(score).sort((a,b)=>score[b]-score[a]);

    let final = result.map(u=>({
        name: u,
        mutuals: score[u]
    }));

    res.json(final);
});

app.get('/path',(req,res)=>{
    res.json(shortestPath(req.query.u,req.query.v));
});

app.get('/network',(req,res)=>{
    res.json(graph);
});
graph = {
  "Alice": ["Bob", "Charlie"],
  "Bob": ["Alice", "David", "Eve"],
  "Charlie": ["Alice", "David"],
  "David": ["Bob", "Charlie", "Eve"],
  "Eve": ["Bob", "David", "Frank"],
  "Frank": ["Eve", "Grace"],
  "Grace": ["Frank"]
};

app.listen(3000,()=>console.log("Server running on http://localhost:3000"));