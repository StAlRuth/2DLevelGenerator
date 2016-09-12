var dimensions = [15, 15];

function Vector(x, y) {
    this.x = x;
    this.y = y;

    this.equals = function(i2) {
        return this.x == i2.x && this.y == i2.y
    }

    this.texts = function() {
        return "[" + this.x + ", " + this.y + "]"
    }
}

function VectorMap() {
    this.vec_ = {}

    this.set = function(vec, value) {
        this.vec_["x" + vec.x + "y" + vec.y] = value
    }

    this.get = function(vec) {
        return this.vec_["x" + vec.x + "y" + vec.y]
    }

    this.keys = function() {
        return Object.keys(this.vec_)
    }

    this.direct = function(a) {
        return this.vec_[a];
    }

    this.directset = function(a, b) {
        this.vec_[a] = b;
    }

    this.reverse = function(a) {
        return new Vector(parseInt(a.split("y")[0].split("x")[1]), parseInt(a.split("y")[1]))
    }

}

function level() {
    this.dimensions_ = new Vector(15,15)
    this.tiles_ = new VectorMap()

    this.getLevelDimensions = function(level) {
        return this.dimensions_;
    }
    
    this.iterate = function(subfunction) {
        keys = this.tiles_.keys();
        for (i = 0; i < keys.length; i++) {
            subfunction(this.tiles_.reverse(keys[i]));
        }
    }
    this.iteratePos = function(subfunction, pos) {
        keys = this.tiles_.keys();
        for (i = 0; i < keys.length; i++) {
            if (subfunction(this.tiles_.direct(keys[i]), this.tiles_.reverse(keys[i]), pos) == "break") {
                break;
            };
        }
    }
    this.movePlayer = function(direction) {

        pos = new Vector(0, 0);

        this.iteratePos(function(h, t, pos) {

            if (h == 2) {
                pos.x = t.x + 0;
                pos.y = t.y + 0;
                return "break";
            }
        }, pos);

        movepos = new Vector(pos.x, pos.y);
        switch (direction) {
            case 0:
                movepos.y += 1;
                break;
            case 1:
                movepos.x += 1;
                break;
            case 2:
                movepos.y -= 1;
                break;
            case 3:
                movepos.x -= 1;
                break;
            default:
                break;
        }
        if (this.tiles_.get(movepos) == 0) {
            document.getElementById("walk").pause()
            document.getElementById("walk").currentTime = 0
            document.getElementById("walk").play();
            
            this.tiles_.set(pos, 0);
            this.tiles_.set(movepos, 2);
        } else if (this.tiles_.get(movepos) == 3) {
        
            document.getElementById("lvlup").pause()
            document.getElementById("lvlup").currentTime = 0
            document.getElementById("lvlup").play();
            
            void document.body.offsetWidth;
            document.body.classList.remove("earthquake2");
            void document.body.offsetWidth;
            document.body.classList.add("earthquake2");
            
            this.tiles_ = new VectorMap();
            
            this.dimensions_.x += 2;
            this.dimensions_.y += 2;
            
            rescale()
            
            if (generator == null) {
                gen = LevelGenerationBase;
                gen(this);
            } else {
                generator.generate(this);
            }
            //We leave now so that we don't rerender the Level again
            return;
        } else {
            document.getElementById("hit").pause()
            document.getElementById("hit").currentTime = 0
            document.getElementById("hit").play();
            document.getElementById("overlay").classList.remove("splat");
            void document.getElementById("overlay").offsetWidth;
            document.getElementById("overlay").classList.add("splat");
        }
        this.updateVertexArray();
    }

    this.getPlayerPos = function() {
        pos = new Vector(0, 0);

        this.iteratePos(function(h, t, pos) {

            if (h == 2) {
                pos.x = t.x + 0;
                pos.y = t.y + 0;
                return "break";
            }
        }, pos);

        if (pos) {
            return pos
        }

        console.warn("No player found!\n");
        return new Vector(0, 0);
    }

    this.getStairPos = function() {
        pos = new Vector(0, 0);

        this.iteratePos(function(h, t, pos) {

            if (h == 3) {
                pos.x = t.x + 0;
                pos.y = t.y + 0;
                return "break";
            }
        }, pos);

        if (pos) {
            return pos
        }

        console.warn("No stairs found!\n");
        return new Vector(0, 0);
    }

    this.vertices_ = []
    this.updateVertexArray = function() {
        this.vertices_ = []
        for (i = 0; i < this.getLevelDimensions(level).x; i++) {
            for (j = 0; j < this.getLevelDimensions(level).y; j++) {
                quad = [];
                quad.push({
                    position: new Vector(i * scale, j * scale)
                });
                quad.push({
                    position: new Vector((i + 1) * scale, j * scale)
                });
                quad.push({
                    position: new Vector((i + 1) * scale, (j + 1) * scale)
                });
                quad.push({
                    position: new Vector(i * scale, (j + 1) * scale)
                });
                switch (this.tiles_.get(new Vector(i, j))) {
                    case 3:
                        color = "rgb(0, 255, 0)";
                        break;
                    case 2:
                        color = "rgb(0, 0, 255)";
                        break;
                    case 1:
                        color = "rgb(255, 0, 0)";
                        break;
                    case 0:
                    default:
                        color = "rgb(0, 0, 0)";
                        break;
                }
                quad[0]["color"] = color;
                quad[1]["color"] = color;
                quad[2]["color"] = color;
                quad[3]["color"] = color;
                this.vertices_.push(quad);
            }
        }
    }

}

var scale = 0;

function rescale() {
    scale = Math.min(window.innerWidth, window.innerHeight) / (level.dimensions_.x);
}

window.onresize = function() {
    rescale();
}

    
window.onkeydown = function(e) {
    e = e || event
    switch (e.keyCode) {
        case 37:
            level.movePlayer(3);
            return false
        case 38:
            level.movePlayer(2);
            return false
        case 39:
            level.movePlayer(1);
            return false
        case 40:
            level.movePlayer(0);
            return false
    }
}

function draw(level) {

    for (i = 0; i < level.vertices_.length; i++) {
        cachedVertex = level.vertices_[i];

        context.beginPath();
        context.moveTo(cachedVertex[0].position.x, cachedVertex[0].position.y);
        for (j = 1; j < 4; j++) {

            context.strokeStyle = cachedVertex[j]["color"];
            context.fillStyle = cachedVertex[j]["color"];
            context.lineWidth = 1;

            context.lineTo(cachedVertex[j].position.x, cachedVertex[j].position.y);

            context.stroke();

        }
        context.closePath();
        context.fill();
    }
}

function rand(val) {
    return Math.floor(Math.random() * val);
}

function LevelGenerationBase() {

    //Passthrough function to separate interface from implementation
    this.operator = function(level) {
        generate(level);
        level.updateVertexArray();
    }

    //Protected interface to allow algorithms to edit the Level.
    this.setLevelTile = function(level, pos, type) {
        level.tiles_.set(pos, type);
    }

    this.getLevelTile = function(level, pos) {
        return level.tiles_.get(pos);
    }

    this.getLevelDimensions = function(level) {
        return level.dimensions_;
    }

    //This function creates a single room with the player and staircase placed inside it.
    //This also serves as an example Level Generation Algorithm.
    this.generate = function(level) {
        for (i = 0; i < this.getLevelDimensions(level).x; i++) {
            for (j = 0; j < this.getLevelDimensions(level).y; j++) {
                if (i == 0 || j == 0 ||
                    i == this.getLevelDimensions(level).x - 1 ||
                    j == this.getLevelDimensions(level).y - 1) {
                    //Setting a tile is straightforward.
                    //The first parameter is the position of the
                    //tile provided as an new Vector, and the
                    //second one is the tile type as char.
                    //Setting a tile to 1 makes it a wall...
                    this.setLevelTile(level, new Vector(i, j), 1);
                } else {
                    //Setting it to 0 makes it empty...
                    this.setLevelTile(level, new Vector(i, j), 0);
                }
            }
        }
        playerPos = new Vector((rand(this.getLevelDimensions(level).x - 2)) + 1,
            (rand(this.getLevelDimensions(level).y - 2)) + 1);
        stairsPos = new Vector((rand(this.getLevelDimensions(level).x - 2)) + 1,
            (rand(this.getLevelDimensions(level).y - 2)) + 1);
        while (playerPos.x == stairsPos.x && playerPos.y == stairsPos.y) {
            stairsPos = new Vector((rand(this.getLevelDimensions(level).x - 2)) + 1,
                (rand(this.getLevelDimensions(level).y - 2)) + 1);
        }
        //Setting it to 2 makes it the player's location
        //I currently do not have any guards against multiple player locations,
        //so please do not add multiple players to the map, as I don't know
        //what will happen :P
        setLevelTile(level, playerPos, 2);
        //Setting it to 3 makes it the Level exit.
        //Ther emay be multiple exits, but please note that a new Level will be
        //generated once any one of them is reached.
        setLevelTile(level, stairsPos, 3);
    }

}

function LevelGenerationKruskal() {
    LevelGenerationBase.call(this);

    this.generate = function(level) {
        //Set up the skeleton - intersections only, everything else is a wall
        for (i = 0; i < this.getLevelDimensions(level).x; i++) {
            for (j = 0; j < this.getLevelDimensions(level).y; j++) {
                if (i % 2 == 1 && j % 2 == 1) {
                    this.setLevelTile(level, new Vector(i, j), 0);
                } else {
                    this.setLevelTile(level, new Vector(i, j), 1);
                }
            }
        }
        //Label every intersection so we can tell when they're connected
        groups = new VectorMap();
        group = 0;
        for (i = 1; i < this.getLevelDimensions(level).x; i += 2) {
            for (j = 1; j < this.getLevelDimensions(level).y; j += 2) {
                groups.set(new Vector(i, j), group);
                group++;
            }
        }
        //This loop is the part of the algorithm that actually creates the maze.
        //On every iteration, it will pick two random and adjacent intersections
        //and check if they are connected.  If they are not, then the wall
        //between them will be removed, and the `groups' map will be updated to
        //show the new connection.  After every pass, the groups algorithm is
        //checked to see if all of the intersections are connected and if so, the
        //algorithm finishes.
        while (true) {
            //Pick random intersection
            int1 = new Vector();
            int1.x = rand((Math.floor(this.getLevelDimensions(level).x / 2)) * 2) + 1;
            int1.y = rand((Math.floor(this.getLevelDimensions(level).y / 2)) * 2) + 1;

            //Pick random direction
            diff = new Vector();
            direction = rand(4);
            switch (direction) {
                case 0:
                    diff.x = 0;
                    diff.y = 1;
                    break;
                case 1:
                    diff.x = 1;
                    diff.y = 0;
                    break;
                case 2:
                    diff.x = 0;
                    diff.y = -1;
                    break;
                case 3:
                    diff.x = -1;
                    diff.y = 0;
                    break;
                default:
                    break;
            }
            hall = new Vector(int1.x + diff.x, int1.y + diff.y);

            //Check if the path has already been cleared
            if (this.getLevelTile(level, hall) == 0) {
                continue;
            }

            id1 = groups.get(int1);
            int2 = new Vector(int1.x + (2 * diff.x), int1.y + (2 * diff.y));

            count = 0;
            cachekeys = groups.keys()
            for (i = 0; i < cachekeys.length; i++) {
                currentCache = groups.reverse(cachekeys[i])
                if (currentCache.equals(int2)) {
                    count++;
                }
            }

            if (count != 1) {
                //We've found the edge, guise!
                continue;
            }

            id2 = groups.get(int2);
            if (id1 == id2) {
                //Already connected, abort.
                continue;
            }

            //Connect the paths
            cachekeys = groups.keys()
            for (i = 0; i < cachekeys.length; i++) {
                cachecache = cachekeys[i]
                currentCache2 = groups.direct(cachecache)
                if (currentCache2 == id2) {
                    groups.directset(cachecache, id1);
                }
            }

            //Clear the path
            this.setLevelTile(level, hall, 0);

            //Check if all intersections are connected
            done = true;
            cachekeys = groups.keys()
            for (i = 0; i < cachekeys.length; i++) {
                cachecache = cachekeys[i]
                currentCache2 = groups.direct(cachecache)
                if (currentCache2 != id1) {
                    done = false;
                }
            }

            if (done == true) {
                break;
            }
        }
        this.setLevelTile(level, new Vector(1, 1), 2);
        this.setLevelTile(level, new Vector((this.getLevelDimensions(level).x - 2), (this.getLevelDimensions(level).y - 2)), 3);
    }
}

LevelGenerationKruskal.prototype = Object.create(LevelGenerationBase.prototype);
LevelGenerationKruskal.prototype.constructor = LevelGenerationKruskal;

function animate() {
    context.clearRect(0, 0, canvas.width, canvas.height);
    level.updateVertexArray()
    draw(level);
    window.requestAnimationFrame(animate);
}

var level, generator;

function main() {
    generator = new LevelGenerationKruskal();
    level = new level();
    level.dimensions_.x = 5
    level.dimensions_.y = 5
    
    rescale();

    generator.generate(level);
    window.requestAnimationFrame(animate);
}

var canvas = document.getElementById('canvas');
canvas.width = window.innerWidth;
canvas.height = window.innerHeight;

var context = canvas.getContext('2d');

main();