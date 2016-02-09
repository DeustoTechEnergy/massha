// GLOBAL CONTROLLER

app.controller('MasshaController', ['$scope', '$rootScope', 'Agent', function($scope, $rootScope, Agent){

    // Variables
    $rootScope.selected_agent_id = false;
    $rootScope.socket_port = 3001;
    $rootScope.hostname = window.location.hostname;
    $rootScope.map = null;
    $rootScope.heat = null;

    // Storage
    $rootScope.environment = {};
    $rootScope.agents = {}; /* {CLASS : { ID : Agent JSON }} */
    $rootScope.layers = {}; /* {CLASS : { ID : GeoJSONLayer }} */

    // Overlay Layers
    $rootScope.overlays = {}; /* {CLASSNAME : FeatureLayerGroup} */

    // Parse each agent
    $rootScope.parseAgent = function(geojson , operation){

        if( !geojson ){
            return;
        }
        
        var id = geojson.properties.id.value;
        var class_name = geojson.properties.class.value;
        if( geojson.properties.current_activity ){
            var date = new Date(geojson.properties.current_time.value);
            if( geojson.properties.current_action ){
                $rootScope.map_info.update( '<div class="panel-body"><h3>' + date.getHours() +':'+ date.getMinutes() +':'+ date.getSeconds() + ' - ' + geojson.properties.current_activity.value + ' - ' + geojson.properties.current_action.value  + '</h3></div>' );
            } else {
                $rootScope.map_info.update( '<div class="panel-body"><h3>' + date.getHours() +':'+ date.getMinutes() +':'+ date.getSeconds() + ' - ' + geojson.properties.current_activity.value + '</h3></div>' );
            }
        }

        switch(operation){
            case 0: /* Create */
            case 1: /* Update */
                /* If new agent class */
                if( !$rootScope.agents[class_name] ){
                    $rootScope.createLayerGroup(class_name);
                }
                /* Delete old from overlays and memory */
                if ($rootScope.agents[ class_name ][ id ]){
                    $rootScope.deleteLayer(class_name , id);
                }
                /* Create geojson layer and add it to overlays */
                $rootScope.createLayer(class_name, id , geojson);
                break;

            case 2: /* Delete */
                /* Delete from agentsLayer and memory */
                if ( $rootScope.agents[ class_name ][ id ] ){
                    $rootScope.deleteLayer(class_name , id);
                }
                break;
        }
    };

    // Create new layergroup
    $rootScope.createLayerGroup = function(class_name){
        $rootScope.agents[class_name] = {};
        $rootScope.layers[ class_name ] = {};
        $rootScope.overlays[ class_name ] = L.featureGroup();
        $rootScope.overlays[ class_name ].addTo( $rootScope.map );
    };

    // Create new layer
    $rootScope.createLayer = function(class_name, id , geojson){
        $rootScope.agents[ class_name ][ id ] = geojson;
        if( $rootScope.selected_agent_id == id ){

            // Apply changes
            $rootScope.$apply();

            // Fit map to agent
            //$rootScope.map.fitBounds(L.geoJson( geojson ).getBounds(), {minZoom : 15 , maxZoom : 20});
        }
        if( geojson.geometry ){
            $rootScope.layers[ class_name ][ id ] = L.geoJson( geojson , {style: $rootScope.getStyle , pointToLayer: $rootScope.pointToLayer,  onEachFeature: $rootScope.onEachHighlightFeature} );
            $rootScope.overlays[ class_name ].addLayer( $rootScope.layers[ class_name ][ id ] );
        }
    };

    // Delete layer
    $rootScope.deleteLayer = function(class_name, id){
            $rootScope.overlays[ class_name ].removeLayer( $rootScope.layers[ class_name ][ id ] );
            delete $rootScope.agents[ class_name ][ id ];
            delete $rootScope.layers[ class_name ][ id ];
    };

    // Parse response
    $rootScope.parseResponse = function(res){
        var json_data = res.data,
            operation = res.operation || 0;

        // data is an array, execute the same operation for all the elements inside it
        if( Object.prototype.toString.call( json_data ) === '[object Array]' ) {
            for(var i in json_data){
                $rootScope.parseAgent(json_data[i], operation );
            }
            // Else, data will only contain one element
        } else {
            $rootScope.parseAgent(json_data, operation );
        }
    };

    // Create one
    $rootScope.createAgent = function(class_name , data){
        Agent.create(class_name , data).success(function(res){ $rootScope.parseResponse( res ); });
    };
    
    // Get One
    $rootScope.getAgent = function(class_name , id ){
        Agent.getOne(class_name , id).success(function(res){ $rootScope.parseResponse( res ); });
    };

    // Update One
    $rootScope.updateAgent = function(class_name , id ){
        Agent.update(class_name , id , $rootScope.agents[class_name][id]).success(function(res){ $rootScope.parseResponse( res ); });
    };

    // Run All
    $rootScope.runAll = function(class_name){
        Agent.runAll(class_name);
    };

    // Run One
    $rootScope.runAgent = function(class_name , id){
        Agent.runOne(class_name , id);
    };

    // Stop All
    $rootScope.stopAll = function(class_name){
        Agent.stopAll(class_name);
    };

    // Stop One
    $rootScope.stopAgent = function(class_name , id){
        Agent.stopOne(class_name , id);
    };

    // Delete All
    $rootScope.deleteAll = function(class_name , id){
        Agent.deleteAll(class_name);
    };

    // Delete one
    $rootScope.deleteAgent = function(class_name , id){
        Agent.deleteOne(class_name , id).success(function(res){$rootScope.deleteLayer(class_name , id)});
    };

    // Locate one
    $rootScope.locateAgent = function(class_name , id, text){
        $rootScope.map.fitBounds( $rootScope.layers[class_name][id].getBounds(), {minZoom : 15 , maxZoom : 20});
        $rootScope.layers[class_name][id].bindPopup('<a href="#/agent/' + class_name + '/' + id + '">' + text + '</a>');
        $rootScope.layers[class_name][id].openPopup();
    };

    // Get agent style
    $rootScope.getStyle = function(feature) {
        var style = feature.properties.style || {};
        return {
            fillColor: style.color ? style.color.value : 'gray',
            weight: style.weight ? style.weight.value : 6,
            opacity: style.border_opacity ? style.border_opacity.value : 0.8,
            color: style.border_color ? style.border_color.value : 'black',
            fillOpacity: style.fill_opacity ? style.fill_opacity.value : 0.6,
            dashArray: style.dash_array ? style.dash_array.value : 1
        };
    };


    // Highlight
    $rootScope.onEachHighlightFeature = function(feature, layer) {
        layer.bindPopup('<a href="#/agent/' + feature.properties.class.value + '/' + feature.properties.id.value + '">' + feature.properties.name.value + '</a>');
    };


    // Point geojson, set marker icon and if draggable
    $rootScope.pointToLayer = function(feature, latlng){
        var title = feature.properties.name.value;
        var style = feature.properties.style || {};
        var icon = L.icon({
            iconUrl: style.icon_url ? style.icon_url.value : 'http://cdn.flaticon.com/png/128/33622.png',
            iconSize: style.icon_size ? style.icon_size : [32, 32],
            iconAnchor: style.icon_anchor ? style.icon_anchor.value : [16, 32],
            popupAnchor: style.popup_anchor ? style.popup_anchor.value : [0, -24]
        });
        return L.marker(latlng, {icon: icon, draggable: false, title: title});
    };


    // Create socket and bind events
    $scope.socket = new WebSocket("ws://" + $rootScope.hostname + ":" + $rootScope.socket_port);
    $scope.socket.onopen = function (event) {};
    $scope.socket.onmessage = function (event) {
        $rootScope.parseResponse( JSON.parse(event.data) );
    }
    
}]);

 /* MAP CONTROLLER */

app.controller('MapController', ['$scope', '$rootScope', 'Environment', 'Agent', function($scope, $rootScope, Environment, Agent){

    // Reset flags
    $rootScope.selected_agent_id = false;

    var locate_class = false;
    var locate_id = false;

    if( !$rootScope.map ){

        // Get environment info
        Environment.get().success(function(res){

            $rootScope.environment = res.data;

            // Create map
            $rootScope.map = L.map('map-canvas' , {fullscreenControl: true}).setView([39.83, -3.4], 7);

            // Create base layers
            $rootScope.baselayers = {
                //Wikimedia: L.tileLayer('https://maps.wikimedia.org/osm-intl/{z}/{x}/{y}.png', { maxZoom: 22, maxNativeZoom: 18 })
                OpenStreetMap: L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', { maxZoom: 22, maxNativeZoom: 19 }).addTo($rootScope.map),
                Toner: L.tileLayer('http://{s}.tile.stamen.com/toner/{z}/{x}/{y}.png', { maxZoom: 22, maxNativeZoom: 18 }),
                WaterColor: L.tileLayer('http://{s}.tile.stamen.com/watercolor/{z}/{x}/{y}.png', { maxZoom: 22, maxNativeZoom: 18 }),
                Satelite: L.tileLayer('http://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/tile/{z}/{y}/{x}.jpg', { maxZoom: 22, maxNativeZoom: 18 })
            };

            // Create overlays
            for(var i in $rootScope.environment.properties['agent-classes']){
                $rootScope.createLayerGroup( $rootScope.environment.properties['agent-classes'][i] );
            }
            
            // Create heatmap layer
            //$rootScope.heat = L.heatLayer([], { maxZoom: 25 }).addTo( $rootScope.map );

            // Add scale
            L.control.scale().addTo( $rootScope.map );
            
            // Add info control
            $rootScope.map_info = L.control({position: 'bottomright'});
            $rootScope.map_info.onAdd = function (map) {
                this._div = L.DomUtil.create('div', 'leaflet-control-layers panel panel-default');
                this._div.innerHTML = '';
                return this._div;
            };
            $rootScope.map_info.update = function(content){ this._div.innerHTML = content; };
            $rootScope.map_info.addTo( $rootScope.map );

            // Fit bounds
            var env_geom = L.geoJson( $rootScope.environment , {style: $rootScope.getStyle} );//.addTo( $rootScope.map );
            $rootScope.map.fitBounds( env_geom.getBounds(), {minZoom : 15 , maxZoom : 20});

            // Layer switcher
            L.control.layers($rootScope.baselayers , $rootScope.overlays).addTo($rootScope.map);

        }).error(function(res){ alert('Error creating map') });
    }

    if( locate_class && locate_id ){

    }
    

}]);

 /* INTRO CONTROLLER */

app.controller('IntroController', ['$scope', '$rootScope', '$routeParams', '$location', 'Environment', function($scope, $rootScope, $routeParams, $location, Environment)
{
    // Reset flags
    $rootScope.selected_agent_id = false;

    /* Get environment info */
    Environment.get().success(function(res){ $rootScope.environment = res.data; }).error(function(res){ $location.path('/') });
}]);

 /* ENVIRONMENT CONTROLLER */

app.controller('EnvironmentController', ['$scope', '$rootScope', '$routeParams', '$location', 'Environment', function($scope, $rootScope, $routeParams, $location, Environment)
{
    // Reset flags
    $rootScope.selected_agent_id = false;

    /* Get environment */
    $scope.reloadEnvironment = function(){ 
        Environment.get().success(function(res){
            
            if( res.data.properties.time && res.data.properties.time.datetime ){ res.data.properties.time.datetime.value = new Date( $rootScope.environment.properties.time.datetime.value ); }
            
            $rootScope.environment = res.data;

            // Create environment edit map
            $scope.map = L.map('agent-map-canvas' , {editable: true, fullscreenControl: true}).setView([39.83, -3.4], 7);
            OpenStreetMap: L.tileLayer('http://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', { maxZoom: 22, maxNativeZoom: 19 }).addTo( $scope.map );
            
            if( $rootScope.environment.geometry ){
                // Clone the geometry to the geometry edit map
                $scope.environment_geometry = L.geoJson( $rootScope.environment , {style: $rootScope.getStyle} ).addTo( $scope.map );
                
                // Fit map to environment
                $scope.map.fitBounds( $scope.environment_geometry.getBounds(), {minZoom : 15 , maxZoom : 20});
            }
            
             // Create environment edit map
            $scope.graph_map = L.map('graph-map-canvas' , {editable: true, fullscreenControl: true}).setView([39.83, -3.4], 7);
            OpenStreetMap: L.tileLayer('http://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', { maxZoom: 22, maxNativeZoom: 19 }).addTo( $scope.graph_map );
            
            // Create environment graph map
            if( $rootScope.environment.properties.graph ){
                // Clone the geometry to the geometry edit map
                $scope.environment_graph = L.geoJson( $rootScope.environment.properties.graph , {style: $rootScope.getStyle} ).addTo( $scope.graph_map );
                
                // Fit map to graph
                $scope.graph_map.fitBounds( $scope.environment_graph.getBounds(), {minZoom : 15 , maxZoom : 20});
            }
            
        }).error(function(res){ $location.path('/') }); };
    
    $scope.updateEnvironment = function(){ Environment.update($rootScope.environment).success(function(res){ $rootScope.environment = res.data; }).error(function(res){ $location.path('/environment') }) };
    $scope.reloadEnvironment();
}]);

/* AGENT CLASSES CONTROLLER */

app.controller('AgentClassesController', ['$scope', '$rootScope', '$routeParams', '$location', 'Environment', function($scope, $rootScope, $routeParams, $location, Environment)
{
    // Reset flags
    $rootScope.selected_agent_id = false;

    /* Get environment info */
    Environment.get().success(function(res){
        $rootScope.environment = res.data;
        $scope.agent_classes = $rootScope.environment.properties['agent-classes'];
        for(var i in $scope.agent_classes){
            $rootScope.createLayerGroup( $scope.agent_classes[i] );
        };
    }).error(function(res){ $location.path('/') });

}]);

 /* AGENTS CONTROLLER */

app.controller('AgentsController', ['$scope', '$rootScope', '$routeParams', '$location', 'Agent', function($scope, $rootScope, $routeParams, $location, Agent)
{
    // Reset flags
    $rootScope.selected_agent_id = false;

    $scope.class_name = $routeParams.class_name;
    $scope.paginated = 0;
    $scope.page_size = 200;
    
    $scope.getNextPaginated = function() {
    /* Get all agents for class with pagination */
        Agent.getAll( $scope.class_name , $scope.paginated * $scope.page_size , ++$scope.paginated * $scope.page_size ).success(function(res){
            $rootScope.parseResponse( res );
            $scope.agents = $rootScope.agents[ $scope.class_name ];
        }).error(function(res){ $location.path('/') });
    }
    $scope.getNextPaginated();
    
}]);

 /* AGENT CONTROLLER */

app.controller('AgentController', ['$scope', '$rootScope', '$routeParams', '$location', 'Agent', function($scope, $rootScope, $routeParams, $location, Agent)
{
    $scope.class_name = $routeParams.class_name;
    $scope.id = $rootScope.selected_agent_id = $routeParams.id;
    $scope.geometry;

    /* Get agent */
    Agent.getOne( $scope.class_name , $scope.id ).success(function(res){
        $rootScope.parseResponse( res );

        /* $rootScope.layers[ $scope.class_name ][ $scope.id ].enableEdit(); */

        // Parse dates
        for(var i in $rootScope.agents[ $scope.class_name ][ $scope.id ].properties){
            if( $rootScope.agents[ $scope.class_name ][ $scope.id ].properties[i].type == "datetime-local" ){
                $rootScope.agents[ $scope.class_name ][ $scope.id ].properties[i].value = new Date( $rootScope.agents[ $scope.class_name ][ $scope.id ].properties[i].value );
            }
        }
        
        // Create agent edit map
        $scope.map = L.map('agent-map-canvas' , {editable: true, fullscreenControl: true}).setView([39.83, -3.4], 7);
        OpenStreetMap: L.tileLayer('http://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', { maxZoom: 22, maxNativeZoom: 19 }).addTo( $scope.map );
        
        // Check if agent has geometry
        if( $rootScope.agents[ $scope.class_name ][ $scope.id ].geometry ){
            
            // Clone the geometry to the geometry edit map
            $scope.geometry = L.geoJson( $rootScope.layers[ $scope.class_name ][ $scope.id ].toGeoJSON() , {style: $rootScope.getStyle , pointToLayer: $rootScope.pointToLayer} ).addTo( $scope.map );
            
             // Fit maps to agent
            $rootScope.map.fitBounds( $rootScope.layers[ $scope.class_name ][ $scope.id ].getBounds(), {minZoom : 15 , maxZoom : 20});
            $scope.map.fitBounds( $scope.geometry.getBounds(), {minZoom : 15 , maxZoom : 20});
        }
    }).error(function(res){ $location.path('/' + $scope.class_name ) });

}]);

 /* NEW AGENT CONTROLLER */

app.controller('AgentsCreateController', ['$scope', '$rootScope', '$routeParams', '$location', 'Agent', function($scope, $rootScope, $routeParams, $location, Agent)
{
    // Reset flags
    $rootScope.selected_agent_id = false;

    $scope.class_name = $routeParams.class_name;
    $scope.agent_amount = 1;
    $scope.agent_properties = {};
    $scope.agent_style = {};
    
    
}]);