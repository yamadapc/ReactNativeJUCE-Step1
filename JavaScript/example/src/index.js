import React, {Component} from 'react';
import {ComponentRegistry, View} from 'react-native-juce';

class HelloWorld extends Component {
  render() {
    return (
      <View>
        Hello World
      </View>
    );
  }
}

ComponentRegistry.register('HelloWorld', HelloWorld);
