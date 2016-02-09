var app = angular.module('massha', ['ngRoute']);

app.config(['$routeProvider', function($routeProvider) {

    $routeProvider

        /* GLOBAL */
        .when('/', {
            templateUrl: 'static/views/intro.html',
            controller: 'IntroController'
        })

        /* ENVIRONMENT */
        .when('/map', {
        })

        /* ENVIRONMENT */
        .when('/environment', {
            templateUrl: 'static/views/environment.html',
            controller: 'EnvironmentController'
        })

        /* AGENTS */
        .when('/agents', {
            templateUrl: 'static/views/agentclasses.html',
            controller: 'AgentClassesController'
        })
        .when('/agents/:class_name', {
            templateUrl: 'static/views/agents.html',
            controller: 'AgentsController'
        })
        .when('/agents/:class_name/create', {
            templateUrl: 'static/views/agentscreate.html',
            controller: 'AgentsCreateController'
        })
        .when('/agent/:class_name/:id', {
            templateUrl: 'static/views/agent.html',
            controller: 'AgentController'
        })


        /* ELSE */
        .otherwise({
            redirectTo: '/'
        });

}]);