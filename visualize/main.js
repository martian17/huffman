let tree = {"cnt":217572,"left":{"cnt":90065,"left":{"cnt":42133,"left":{"cnt":20095,"left":{"char":"o","cnt":10044,"code":111},"right":{"cnt":10051,"left":{"cnt":4942,"left":{"char":"b","cnt":2449,"code":98},"right":{"char":"c","cnt":2493,"code":99}},"right":{"char":"l","cnt":5109,"code":108}}},"right":{"char":"e","cnt":22038,"code":101}},"right":{"cnt":47932,"left":{"cnt":22317,"left":{"cnt":10827,"left":{"cnt":5191,"left":{"cnt":2520,"left":{"cnt":1211,"left":{"cnt":605,"left":{"char":"E","cnt":299,"code":69},"right":{"char":"6","cnt":306,"code":54}},"right":{"char":";","cnt":606,"code":59}},"right":{"cnt":1309,"left":{"cnt":619,"left":{"char":"7","cnt":307,"code":55},"right":{"cnt":312,"left":{"char":"H","cnt":156,"code":72},"right":{"char":"P","cnt":156,"code":80}}},"right":{"char":"I","cnt":690,"code":73}}},"right":{"cnt":2671,"left":{"char":".","cnt":1316,"code":46},"right":{"char":"v","cnt":1355,"code":118}}},"right":{"cnt":5636,"left":{"char":"y","cnt":2677,"code":121},"right":{"cnt":2959,"left":{"cnt":1426,"left":{"cnt":704,"left":{"char":"5","cnt":346,"code":53},"right":{"char":"L","cnt":358,"code":76}},"right":{"char":"4","cnt":722,"code":52}},"right":{"char":" ","cnt":1533,"code":10}}}},"right":{"char":"s","cnt":11490,"code":115}},"right":{"cnt":25615,"left":{"char":"n","cnt":12650,"code":110},"right":{"cnt":12965,"left":{"cnt":6131,"left":{"char":"w","cnt":2995,"code":119},"right":{"cnt":3136,"left":{"char":" ","cnt":1533,"code":9},"right":{"cnt":1603,"left":{"char":"3","cnt":774,"code":51},"right":{"cnt":829,"left":{"cnt":407,"left":{"cnt":197,"left":{"char":"W","cnt":94,"code":87},"right":{"cnt":103,"left":{"char":"Z","cnt":47,"code":90},"right":{"cnt":56,"left":{"char":"K","cnt":25,"code":75},"right":{"cnt":31,"left":{"cnt":14,"left":{"char":"(","cnt":6,"code":40},"right":{"char":"-","cnt":8,"code":45}},"right":{"char":"q","cnt":17,"code":113}}}}},"right":{"char":"O","cnt":210,"code":79}},"right":{"char":"J","cnt":422,"code":74}}}}},"right":{"cnt":6834,"left":{"cnt":3317,"left":{"char":"A","cnt":1624,"code":65},"right":{"char":"p","cnt":1693,"code":112}},"right":{"char":"u","cnt":3517,"code":117}}}}}},"right":{"cnt":127507,"left":{"cnt":54947,"left":{"cnt":26437,"left":{"char":"h","cnt":13103,"code":104},"right":{"char":"t","cnt":13334,"code":116}},"right":{"cnt":28510,"left":{"char":"a","cnt":13858,"code":97},"right":{"cnt":14652,"left":{"cnt":7238,"left":{"char":"f","cnt":3579,"code":102},"right":{"cnt":3659,"left":{"cnt":1815,"left":{"cnt":902,"left":{"cnt":431,"left":{"char":"B","cnt":214,"code":66},"right":{"char":"D","cnt":217,"code":68}},"right":{"cnt":471,"left":{"cnt":225,"left":{"char":"C","cnt":112,"code":67},"right":{"char":"M","cnt":113,"code":77}},"right":{"cnt":246,"left":{"char":"N","cnt":115,"code":78},"right":{"cnt":131,"left":{"char":"z","cnt":65,"code":122},"right":{"char":"j","cnt":66,"code":106}}}}},"right":{"char":"k","cnt":913,"code":107}},"right":{"char":"G","cnt":1844,"code":71}}},"right":{"char":"r","cnt":7414,"code":114}}}},"right":{"cnt":72560,"left":{"cnt":34295,"left":{"cnt":16132,"left":{"cnt":7566,"left":{"char":",","cnt":3682,"code":44},"right":{"char":"m","cnt":3884,"code":109}},"right":{"cnt":8566,"left":{"cnt":4071,"left":{"char":"g","cnt":2004,"code":103},"right":{"cnt":2067,"left":{"char":"2","cnt":1011,"code":50},"right":{"cnt":1056,"left":{"cnt":513,"left":{"char":"T","cnt":249,"code":84},"right":{"char":"\'","cnt":264,"code":39}},"right":{"cnt":543,"left":{"char":"8","cnt":269,"code":56},"right":{"char":"S","cnt":274,"code":83}}}}},"right":{"cnt":4495,"left":{"char":":","cnt":2204,"code":58},"right":{"cnt":2291,"left":{"char":"1","cnt":1144,"code":49},"right":{"cnt":1147,"left":{"cnt":557,"left":{"char":"0","cnt":275,"code":48},"right":{"char":"R","cnt":282,"code":82}},"right":{"cnt":590,"left":{"char":"9","cnt":293,"code":57},"right":{"cnt":297,"left":{"cnt":148,"left":{"char":"x","cnt":68,"code":120},"right":{"cnt":80,"left":{"char":"F","cnt":39,"code":70},"right":{"cnt":41,"left":{"char":"U","cnt":19,"code":85},"right":{"cnt":22,"left":{"cnt":8,"left":{"char":"!","cnt":2,"code":33},"right":{"char":")","cnt":6,"code":41}},"right":{"char":"Y","cnt":14,"code":89}}}}},"right":{"char":"?","cnt":149,"code":63}}}}}}}},"right":{"cnt":18163,"left":{"char":"d","cnt":8985,"code":100},"right":{"char":"i","cnt":9178,"code":105}}},"right":{"char":" ","cnt":38265,"code":32}}}};


let max = function() {
    let m = -Infinity;
    for (let i = 0; i < arguments.length; i++) {
        if (arguments[i] > m) {
            m = arguments[i];
        }
    }
    return m;
};

let planTree = function(tree) {
    if (tree.left) { //has children
        let left = planTree(tree.left);
        let right = planTree(tree.right);
        return {
            width: left.width + right.width + 1,
            height: max(left.height, right.height) + 1,
            middle: left.width + 0.5,
            cnt: tree.cnt,
            left: left,
            right: right
        }
    } else {
        return {
            width: 1,
            height: 1,
            middle: 0.5,
            char: getChar(tree),
            cnt: tree.cnt
        }
    }
};

let getChar = function(tree){
    if(tree.code === 9){
        return "\\t";
    }else if(tree.code === 10){
        return "\\n";
    }else if(tree.code === 32){
        return "SPACE";
    }else{
        return tree.char;
    }
};


let drawTree = function(ctx, plan, xcoef, ycoef, xoff, yoff) {
    if (plan.char) { //leaf node
        ctx.font = "9px Arial";
        ctx.fillText(plan.cnt, xoff + plan.middle * xcoef, yoff + 0.5 * ycoef);
        ctx.font = "14px Arial";
        ctx.fillText(plan.char, xoff + plan.middle * xcoef, yoff + 0.9 * ycoef); //char height:0.2
    } else { //stem node
        ctx.font = "9px Arial";
        ctx.fillText(plan.cnt, xoff + plan.middle * xcoef, yoff + 0.5 * ycoef);
        //left line
        ctx.beginPath();
        ctx.moveTo(xoff + plan.middle * xcoef, yoff + 0.7 * ycoef);
        ctx.lineTo(xoff + plan.left.middle * xcoef, yoff + 1.3 * ycoef);
        ctx.stroke();
        //right line
        ctx.beginPath();
        ctx.moveTo(xoff + plan.middle * xcoef, yoff + 0.7 * ycoef);
        ctx.lineTo(xoff + (plan.left.width + 1 + plan.right.middle) * xcoef, yoff + 1.3 * ycoef);
        ctx.stroke();
        drawTree(ctx, plan.left, xcoef, ycoef, xoff, yoff + ycoef);
        drawTree(ctx, plan.right, xcoef, ycoef, xoff + (plan.left.width + 1) * xcoef, yoff + ycoef);

    }
};


let main = async function() {
    let canvas = document.createElement("canvas");
    document.body.appendChild(canvas);
    canvas.width = 2100;//window.innerWidth;
    canvas.height = 620;//window.innerHeight+20;
    let width = 2000;//canvas.width;
    let height = 600//window.innerHeight;//canvas.height;

    let ctx = canvas.getContext("2d");
    ctx.fillStyle = "#fff";
    ctx.fillRect(0,0,canvas.width,canvas.height);
    ctx.fillStyle = "#000";
    ctx.textBaseline = "middle";
    ctx.textAlign = "center";
    let plan = planTree(tree);
    console.log(plan);
    drawTree(ctx, plan, width / plan.width, height / plan.height, 50, 0);
};



main();